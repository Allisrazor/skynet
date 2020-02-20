unit keras;

{$mode objfpc}{$H+}

interface
const
{$ifdef FPC}
  KERAS_EXPORT = 'libsnkeras.so';
{$else}
  KERAS_EXPORT = 'snkeras.dll';
{$endif}

type
  TStatus = Integer;  // Статус выполнения задачи
  TActivation = Integer; // Функции активации
  TOptimizer = Integer; // Метод оптимизации
  TBatchNormType = Integer; // Типы нормализации наборов
  TLossType = Integer; // Типы функции потерь
  TPoolType = Integer; // Типы объединения

  // Размер данных в слое
  pLayerSize = ^TLayerSize;
  TLayerSize = Record
     w: Cardinal;  // Ширина
     h: Cardinal;  // Длина
     ch: Cardinal; // Канальность
     bsz: Cardinal;// Количество данных  в наборе
     end;

const
  STATUS_OK = 0; ///< Успешное выполнение задачи
  STATUS_FAILURE = 1; ///< Критическая ошибка
  STATUS_WARNING = 2; ///< Задача выполнена, но были проблемы

  ACTIV_NONE = -1;      // Без функции активации
  ACTIV_SIGMOID = 0;    // Сигмойдная функция
  ACTIV_RELU = 1;       // Функция RELU
  ACTIV_LEAKY_RELU = 2; // Функция LEAKY RELU
  ACTIV_ELU = 3;        // Функция ELU

  OPTIM_SGD = 0;
  OPTIM_SGD_MOMENT = 1;
  OPTIM_ADAGRAD = 2;
  OPTIM_RMSPROP = 3;
  OPTIM_ADAM = 4;

  BATCH_NONE = -1;         // Без нормализации
  BATCH_BEFORE_ACTIVE = 0; // до активации
  BATCH_POST_ACTIVE = 1;   // после активации

  LOSS_SOFTMAX_CROSS_ENTROPY = 0;
  LOSS_BINARY_CROSS_ENTROPY = 1;
  LOSS_REGRESSION_MSE = 2;
  LOSS_USER_LOSS = 3;  // Пользовтельская функция потерь

  POOL_MAX = 0;
  POOL_AVG = 1;

// Создание модели
// return Статус создания модели
Function createModel(): TStatus;  cdecl; external KERAS_EXPORT;

// addInput Добавляет сходной слой
// name Имя слоя
// nodes узли с которыми связан слой (через пробел)
// Статус добавления входного слоя в модель
Function addInput(name: PChar; nodes: PChar): TStatus; cdecl; external KERAS_EXPORT;

// addConvolution Добавляет сверточный слой
// name Имя слоя
// nodes узлы с которыми связан слой (через пробел)
// return Статус добавления слоя в модель
Function addConvolution(name: PChar; nodes: PChar; filters_: Cardinal;
                        act_: TActivation = ACTIV_RELU;
                        opt_: TOptimizer = OPTIM_ADAM;
                        dropOut_: Single = 0.0;
                        bnorm_: TBatchNormType = BATCH_NONE;
                        fWidth_: Cardinal = 3;
                        fHeight_: Cardinal = 3;
                        padding_: Integer = 0;
                        stride_: Cardinal = 1;
                        dilate_: Cardinal = 1;
                        gpuDeviceId_: Cardinal = 0
                        ): TStatus; cdecl; external KERAS_EXPORT;

 // addDeconvolution Добавляет анти-сверточный слой
 // name Имя слоя
 // nodes узлы с которыми связан слой (через пробел)
 // filters_ Количество нейронов
 // act_ функция активации
 // opt_ Оптимизатор
 // dropOut_
 // bnorm_
 // fWidth_
 // fHeight_
 // stride_
 // gpuDeviceId_ ID видеокарты для рсчетов
 // return Статус добавления слоя в модель
Function addDeconvolution(name: PChar; nodes: PChar; filters_: Cardinal;
                          act_: TActivation = ACTIV_RELU;
                          opt_: TOptimizer = OPTIM_ADAM;
                          dropOut_: Single = 0.0;
                          bnorm_: TBatchNormType = BATCH_NONE;
                          fWidth_: Cardinal = 3;
                          fHeight_: Cardinal = 3;
                          stride_: Cardinal = 1;
                          gpuDeviceId_: Cardinal = 0
                          ) : TStatus; cdecl; external KERAS_EXPORT;


// brief addPooling Добавляет слой объединения
// name Имя слоя
// nodes узлы с которыми связан слой (через пробел)
// kernel_
// stride_
// pool_
// gpuDeviceId_ ID видеокарты для рсчетов
// return Статус добавления слоя в модель
Function addPooling(name: PChar; nodes: PChar; kernel_: Cardinal;
                    stride_: Cardinal;
                    pool_: TPoolType = POOL_MAX;
                    gpuDeviceId_: Cardinal = 0
                    ) : TStatus; cdecl; external KERAS_EXPORT;

// addDense Добавляет плотный слой
// name Имя слоя
// nodes узли с которыми связан слой (через пробел)
// return Статус добавления слоя в модель
Function addDense(name: PChar; nodes: PChar; units_: Cardinal;
                  act_: TActivation = ACTIV_RELU;
                  opt_: TOptimizer = OPTIM_ADAM;
                  dropOut_: Single = 0.0;
                  bnorm_: TBatchNormType = BATCH_NONE;
                  gpuDeviceId_: Cardinal = 0
                  ): TStatus; cdecl; external KERAS_EXPORT;

// addLossFunction Добавляет функцию потерь в модель
// name Имя слоя
// nodes  узли с которыми связан слой (через пробел)
// loss_ тип функции потерь
// return Статус добавления слоя в модель
Function addLossFunction(name: PChar; nodes: PChar;
                         loss_: TLossType): TStatus; cdecl; external KERAS_EXPORT;

// netArchitecture Записывает архитектуру сети в JSON виде
// buffer Буфер для записи
// length Размер буфера
//return Статус вывода архитектуры
Function netArchitecture(buffer: PChar; length: Cardinal
                         ): TStatus; cdecl; external KERAS_EXPORT;

// fit Запуск упрощенной тренировки для известных наборов
// data тренировочные данные
// dataSize количество тренировочных данных
// label метки тренировочных данных
// labelsSize количество меток
// epochs @brief Эпохи
// classes вероятностное распределения на N классов
// Статус тренировки
Function fit(data: Single; dataSize: TLayerSize;
             label_: Byte; labelsSize: TLayerSize;
             epochs: Cardinal; learningRate: Single): TStatus; cdecl; external KERAS_EXPORT;

// evaluate Проверка с тестовым сетом
// data тестовые данные
// dataSize количество тестовых данных
// label метки тестовые данных
// labelsSize количество меток
// verbose уровень подробности
// return Статус тестирования
Function evaluate(data: Single; dataSize: TLayerSize;
                  label_: Byte; labelsSize: TLayerSize;
                  verbose: Cardinal
                  ): TStatus; cdecl; external KERAS_EXPORT;

// saveModel Сохраняет модель с весами
// filename имя с путем
// return Статус сохранения
Function saveModel(filename: PChar): TStatus; cdecl; external KERAS_EXPORT;

// saveModel Загружает модель с весами
// filename имя с путем
// return Статус загрузки
Function loadModel(filename: PChar): TStatus; cdecl; external KERAS_EXPORT;

// lastError Вывод последней ошибки в буфер
// buffer Буфер для текста ошибки
// length размер буфера
Procedure lastError(buffer: PChar; length: Cardinal); cdecl; external KERAS_EXPORT;

// printLastError Вывод последней ошибки в stdout
Procedure printLastError(status: TStatus); cdecl; external KERAS_EXPORT;

implementation

end.

