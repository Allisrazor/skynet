#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "keras.h"
#include "dataset.h"

#define KR_CHECK(A) printLastError(A)

using namespace std;

int main()
{
  //----- Создание графа модели -----
  KR_CHECK(createModel());
  KR_CHECK(addInput("Input", "D1"));
  KR_CHECK(addDense("D1", "D2", 200));
  KR_CHECK(addDense("D2", "LS", 10));
  KR_CHECK(addLossFunction("LS", "Output", LOSS_SOFTMAX_CROSS_ENTROPY));
  //=================================

  //----- Вывод модели --------------
  char buffer[1024];
  KR_CHECK(netArchitecture(buffer, 1024));
  cout<<buffer<<endl;
  //=================================
//  KR_CHECK(readMnist("data"));
  KR_CHECK(readMnistTrain("data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte", 0));
  KR_CHECK(readMnistTest("data/t10k-images-idx3-ubyte", "data/t10k-labels-idx1-ubyte"));
  int classCnt = 10;  // выход: вероятностное распределение на 10 классов

  //----- Тренировка ----------------
#define __TRAINING__
  LayerSize layerDataSize, layerLabelSize;
#if defined(__TRAINING__)
  MnistDATA trainData = mnistTrainParams();
  layerDataSize.bsz = trainData.quantity;
  layerDataSize.ch = 1;
  layerDataSize.w = trainData.rows;
  layerDataSize.h = trainData.cols;
  layerLabelSize.bsz = trainData.quantity;
  layerLabelSize.w = classCnt;
  layerLabelSize.h = 1;
  layerLabelSize.ch = 1;

//  KR_CHECK(fit(trainData.data, layerDataSize, trainData.labels, layerLabelSize, 10, 0.001f));
  KR_CHECK(trainCreate(trainData.data, layerDataSize, trainData.labels, layerLabelSize));
  for (int i = 0; i < 10; ++i) {
    KR_CHECK(trainStep(0.001f, layerDataSize));
    cout<<lastAccurateSum()<<endl;
  }
  KR_CHECK(trainStop());
  cout<<lastAccurateSum()<<endl;
  KR_CHECK(saveModel("w.dat"));

  //----- Тестирование --------------
#elif defined(__TESTING__)
  MnistDATA testData = mnistTestParams();
  layerDataSize.bsz = testData.quantity;
  layerDataSize.ch = 1;
  layerDataSize.w = testData.rows;
  layerDataSize.h = testData.cols;
  layerLabelSize.bsz = testData.quantity;
  layerLabelSize.w = classCnt;
  layerLabelSize.h = 1;
  layerLabelSize.ch = 1;

  KR_CHECK(loadModel("w.dat"));
  KR_CHECK(evaluate(testData.data, layerDataSize, testData.labels, layerLabelSize, 2));

#endif //__TRAINING__ or __TESTING__


  return 0;
}
