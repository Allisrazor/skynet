#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <map>

#include "keras.h"
#include "dataset.h"

#include "workingtitanic.h"

using namespace std;

namespace titanic {

  int workingTitanic(bool isTraining)
  {
    const char * netName = "08_titanic.json";
    const char * weightName = "08_titanic.dat";

    LayerSize layerDataSize, layerLabelSize;
    float * data = nullptr;
    //----- Тренировка -----
    if (isTraining) {
      //----- Создание модели -----
      int modelID = createModel();
      addInput(modelID, "Input", "D1");
      addDense(modelID, "D1", "D2", 400, ACTIV_SIGMOID);
      addDense(modelID, "D2", "LS", 2);
      addLossFunction(modelID, "LS", "Output", LOSS_SOFTMAX_CROSS_ENTROPY);
      //=================================

      //----- Вывод модели --------------
      char buffer[2048];
      netArchitecture(modelID, buffer, sizeof(buffer));
      cout<<buffer<<endl;
      //=================================
      uint8_t * label = nullptr;
      Status retStatus = titanicTrainData("../data/08_titanic/train.csv", &data, &label, &layerDataSize, &layerLabelSize, 0, 3);
      if(retStatus != STATUS_OK) {
        char errBuffer[1024];
        dsLastError(errBuffer, sizeof (errBuffer));
        cout<<"(ERROR): "<<errBuffer<<endl;
        return 1;
      }
      float accuracySum = 0.f;
      const int epoche = 130, reset = 10;

      for(int i = 0; i < epoche; ++i) {
        if(i % reset == 0) {
          accuracySum = 0.f;
        }
        //----- Эпохи не важны (1 штука) -----
        float accuracy = 0.f;
        fit(modelID, data, layerDataSize, label, layerLabelSize, 100, 0.01f, accuracy);
        accuracySum += accuracy;
        cout<<"EPOCHE "<<i<<" ==> "<<accuracySum / ((i % reset) + 1)<<endl;
      }
      saveModel(modelID, netName, weightName);
      delete [] label;
      deleteModel(modelID);
    }
    //----- Тестирование --------------
    else {
      int modelID = createModel(netName, weightName);
      for(unsigned int i = 0; i < 100; ++i) {
        Status retStatus = titanicTrainData("../data/08_titanic/test.csv", &data, nullptr, &layerDataSize, &layerLabelSize, 1, i);
        if(retStatus != STATUS_OK) {
          char errBuffer[1024];
          dsLastError(errBuffer, sizeof (errBuffer));
          cout<<"(ERROR): "<<errBuffer<<endl;
          return 1;
        }

        loadWeight(modelID, "08_titanic.dat");
        float accuracy = 0.f;
        unsigned char * ans = new unsigned char[layerDataSize.bsz];
        evaluate(modelID, data, layerDataSize, nullptr, layerLabelSize, 2, accuracy, ans);
        cout<<"Testing: "<<(int)ans[0]<<endl;
        delete [] ans;
      }
      deleteModel(modelID);
    }
    delete [] data;

    return 0;
  }

}


