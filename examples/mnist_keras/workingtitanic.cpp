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

    LayerSize layerDataSize, layerLabelSize;
    float * data; uint8_t * label;
    titanicTrainData("../data/08_titanic/train.csv", &data, &label, &layerDataSize, &layerLabelSize);
    //----- Тренировка -----
    if (isTraining) {

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
      saveModel(modelID, "08_titanic.dat");
    }
    //----- Тестирование --------------
    else {
      loadModel(modelID, "08_titanic.dat");
      float accuracy = 0.f;
      evaluate(modelID, data, layerDataSize, label, layerLabelSize, 2, accuracy);
      cout<<"Testing: "<<accuracy<<endl;
    }
    delete [] data;
    delete [] label;
    deleteModel(modelID);
    return 0;
  }

}


