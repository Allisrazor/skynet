
#include "../stdafx.h"
#include "SNOperator/src/structurs.h"
#include "SNOperator/src/mathFunctions.h"

using namespace std;
using namespace SN_Base;


#ifdef SN_CPU

void bwdFullyConnected(size_t kernel, snFloat* weight,
	snSize insz, snFloat* input, snFloat* gradIn, snFloat* gradOut, snFloat* dWOut){

	size_t imSz = insz.w * insz.h * insz.d + 1;
	
	// Градиент по весам
	// dW = αIn^T * GrIn + βdW
	// In - матрица вход данных с предыд слоя
	// GrIn - матрица градиентов со след слоя
	cblas_sgemm(CBLAS_ORDER::CblasRowMajor,
		CBLAS_TRANSPOSE::CblasTrans,
		CBLAS_TRANSPOSE::CblasNoTrans,
		imSz,                          // In, строк, кол-во вх значений (+1 - X0)     
		kernel,                        // GrIn, столбцов, кол-во скрытых нейронов 
		insz.n,                        // In, столбцов. GrIn, строк, размер батча                   
		1.0F / insz.n,                 // α коэф 
		input,                         // In, - вх данные - вх значения пришедшие с предыд слоя
		imSz,                          // In, - шаг до след
		gradIn,                        // GrIn - градиент пришедший со след слоя
		kernel,                        // GrIn - шаг до след
		0.0F,                          // β коэф 
		dWOut,                         // dW, выходные данные - градиент по весам
		kernel);                       // dW, шаг до след
		
	// Градиент для предыд слоя
	// GrOut = αGrIn * W^T + βGrOut
	// GrIn - матрица градиентов со след слоя
	// W - веса
	cblas_sgemm(CBLAS_ORDER::CblasRowMajor,
		CBLAS_TRANSPOSE::CblasNoTrans,
		CBLAS_TRANSPOSE::CblasTrans,
		insz.n,                        // GrIn, строк, размер батча     
		imSz - 1,                      // W, столбцов, кол-во вх значений 
		kernel,                        // GrIn, столбцов. W, строк, кол-во скрытых нейронов                 
		1.0F,                          // α, коэф 
		gradIn,                        // GrIn, градиент пришедший со след слоя
		kernel,                        // GrIn, шаг до след X (X21 - X11) 
		weight + kernel,               // W, веса
		kernel,                        // W, шаг до след W (W21 - W11) 
		0.0F,                          // β, доп коэф 
		gradOut,                       // GrOut, градиент для предыд слоя
		imSz - 1);                     // GrOut, шаг до след Y (Y21 - Y11) 
}

void bwdConvolution(size_t kernel, size_t krnWidth, size_t krnHeight, size_t stride, 
	snFloat* weight, snSize insz, snFloat* input, snSize outsz, snFloat* gradIn, snFloat* gradOut, snFloat* dWeightOut){

	size_t wStepByD = krnWidth * krnHeight,              // шаг весов по входу
		   wStepByK = krnWidth * krnHeight * insz.d,     // шаг весов по выходу
		   inStepByD = insz.w * insz.h,                  // шаг вх слоя по входу
		   inStepByN = insz.w * insz.h * insz.d,         // шаг вх слоя по батчу
		   outStepByD = outsz.w * outsz.h,               // шаг вых слоя по выходу
		   outStepByN = outsz.w * outsz.h * outsz.d;     // шаг вых слоя по батчу

	vector<snFloat> inBuff(insz.d, 0), ginBuff(kernel, 0), goutBuff(insz.d, 0);

	memset(gradOut, 0, inStepByN * insz.n * sizeof(snFloat));
	memset(dWeightOut, 0, wStepByK * kernel * sizeof(snFloat));

	// по батчу
	for (size_t n = 0; n < insz.n; ++n){

//#pragma omp parallel for private(inBuff, ginBuff, goutBuff)
		for (size_t p = 0; p < outStepByD; ++p){

			size_t ox = p % outsz.w, oy = p / outsz.w,
				posW = ox * stride, posH = oy * stride;
					
			snFloat* pGrIn = gradIn + ox + oy * outsz.w + n * outStepByN;
			snFloat* pdW = dWeightOut + wStepByK;

			for (size_t k = 0; k < kernel; ++k){
					
				pdW += k * wStepByK + k;

				ginBuff[k] = *pGrIn;				
				
				*pdW += *pGrIn;      // + bias

				pGrIn += outStepByD;
			}
					
			// ядро свертки
			for (size_t c = 0; c < (krnWidth * krnHeight); ++c){

				size_t cx = c % krnWidth, cy = c / krnWidth;
				snFloat* pIn = input + (cx + posW) + (cy + posH) * insz.w + n * inStepByN;				
				snFloat* pW = weight + cx + cy * krnWidth;
				snFloat* pdW = dWeightOut + cx + cy * krnWidth;

				for (size_t d = 0; d < insz.d; ++d){
					inBuff[d] = *pIn;
					pIn += inStepByD;
				}

				memset(goutBuff.data(), 0, insz.d * sizeof(snFloat));

				// по всем вых слоям
				for (size_t k = 0; k < kernel; ++k){

					// по всем вх слоям
					snFloat gin = ginBuff[k];
					for (size_t d = 0; d < insz.d; ++d){
						goutBuff[d] += gin * (*pW);
						*pdW += gin * inBuff[d];
						
						pW += wStepByD;
						pdW += wStepByD;
					}

					pW += 1;           // bias;	
					pdW += 1;
				}

				snFloat* pGrOut = gradOut + (cx + posW) + (cy + posH) * insz.w + n * inStepByN;

				for (size_t d = 0; d < insz.d; ++d){
					*pGrOut += goutBuff[d];
					pGrOut += inStepByD;
				}				
			}					
		}
	}
		
	for (size_t i = 0; i < (wStepByK * kernel); ++i)
		dWeightOut[i] /= kernel;

	if (insz.n > 1){
		for (size_t i = 0; i < (wStepByK * kernel); ++i)
			dWeightOut[i] /= insz.n;
	}

	bool ret = false;
}   

void bwdBatchNorm(snSize insz, snFloat* gradIn, snFloat* gradOut, batchNormParam prm){
	// https://kevinzakka.github.io/2016/09/14/batch_normalization/

	size_t inSz = insz.w * insz.h * insz.d, bsz = insz.n;

	/// ∂f/∂β = ∑∂f/∂y
	cblas_sgemv(CBLAS_ORDER::CblasRowMajor,
		CBLAS_TRANSPOSE::CblasTrans,
		bsz,                          // ∂f/∂y, строк - размер батча
		inSz,                         // ∂f/∂y, столбцов 
		1.F,                          // коэф
		gradIn,                       // ∂f/∂y, данные
		inSz,                         // ∂f/∂y, шаг до след
		prm.onc,                      // 1й вектор
		1,                            // 1й вектор, шаг движения по вектору
		0.0,                          // коэф
		prm.dSchift,                  // ∂f/∂β, результ
		1);
	
	/// ∂f/∂γ = ∑∂f/∂y ⋅ ^x
	for (size_t i = 0; i < inSz; ++i){
		
		snFloat* igr = gradIn + i, *norm = prm.norm + i, dScale = 0.F;
		for (size_t j = 0; j < bsz; ++j){
			
			dScale += igr[0] * norm[0];

			norm += inSz;
			igr += inSz;
		}
		prm.dScale[i] = dScale;
	}

	/// ∂f/∂x = (m⋅γ⋅∂f/∂y − γ⋅∂f/∂β − ^x⋅γ⋅∂f/∂γ) / m⋅σ2
	for (size_t i = 0; i < inSz; ++i){

		snFloat* igr = gradIn + i, *ogr = gradOut + i, *norm = prm.norm + i,
			varce = prm.varce[i] * bsz, scale = prm.scale[i] / varce,
			dSchift = prm.dSchift[i], dScale = prm.dScale[i];
		for (size_t j = 0; j < bsz; ++j){

			*ogr = scale * (*igr * bsz - dSchift - *norm * dScale);

			norm += inSz;
			igr += inSz;
			ogr += inSz;
		}

		prm.schift[i] -= prm.dSchift[i] * prm.lr;
		prm.scale[i] -= prm.dScale[i] * prm.lr;
	}	
}


#endif