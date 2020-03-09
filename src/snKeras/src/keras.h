#ifndef KERAS_H
#define KERAS_H

#include "snkeras_global.h"

#if defined(__cplusplus)
extern "C" {
#endif //__cplusplus

#define STATUS_OK 0 ///< Успешное выполнение задачи
#define STATUS_FAILURE 1 ///< Критическая ошибка
#define STATUS_WARNING 2 ///< Задача выполнена, но были проблемы

typedef int Status; ///< @brief Статус выполнения задачи

#define ACTIV_NONE -1 // Без функции активации
#define ACTIV_SIGMOID 0 // Сигмойдная функция
#define ACTIV_RELU 1  // Функция RELU
#define ACTIV_LEAKY_RELU 2 // Функция LEAKY RELU
#define ACTIV_ELU 3// Функция ELU

typedef int Activation; ///< @brief Функции активации

#define OPTIM_SGD 0
#define OPTIM_SGD_MOMENT 1
#define OPTIM_ADAGRAD 2 // С накоплением импульсаа
#define OPTIM_RMSPROP 3
#define OPTIM_ADAM 4  // накопление движения

typedef int Optimizer; ///< @brief Функции Оптимизации

#define BATCH_NONE -1 // Без нормализации
#define BATCH_BEFORE_ACTIVE 0 // до активации
#define BATCH_POST_ACTIVE 1 // после активации

typedef int BatchNormType; ///< @brief Типы нормализации наборов

#define LOSS_SOFTMAX_CROSS_ENTROPY 0
#define LOSS_BINARY_CROSS_ENTROPY 1
#define LOSS_REGRESSION_MSE 2
#define LOSS_USER_LOSS 3  // Пользовтельская функция потерь

typedef int LossType; ///< @brief Типы функции потерь

#define POOL_MAX 0
#define POOL_AVG 1

typedef int PoolType; ///< @brief Типы объединения

#define SUMMATOR_SUMM 0
#define SUMMATOR_DIFF 1
#define SUMMATOR_MEAN 2

typedef int SummatorType; ///< @brief Типы суммирования

/** @brief Размер данных в слое */
struct LayerSize
{
  unsigned int w; ///< @brief Ширина
  unsigned int h; ///< @brief Длина
  unsigned int ch;  ///< @brief Канальность
  unsigned int bsz; ///< @brief Количество данных  в наборе
};

/**
 * @brief CreateModel Создание модели
 * @return Статус создания модели
 */
KERAS_EXPORT Status createModel();

/**
 * @brief addInput Добавляет сходной слой
 * @param name Имя слоя
 * @param nodes узли с которыми связан слой (через пробел)
 * @return Статус добавления входного слоя в модель
 */
KERAS_EXPORT Status addInput(const char * name, const char * nodes);

/**
 * @brief addConvolution Добавляет сверточный слой
 * @param name Имя слоя
 * @param nodes узлы с которыми связан слой (через пробел)
 * @return Статус добавления слоя в модель
 */
KERAS_EXPORT Status addConvolution(const char *name, const char *nodes, unsigned int filters_,
                                Activation act_ = ACTIV_RELU,
                                Optimizer opt_ = OPTIM_ADAM,
                                float dropOut_ = 0.0f,
                                BatchNormType bnorm_ = BATCH_NONE,
                                unsigned int fWidth_ = 3,
                                unsigned int fHeight_ = 3,
                                int padding_ = 0,
                                unsigned int stride_ = 1,
                                unsigned int dilate_ = 1,
                                unsigned int gpuDeviceId_ = 0);

/**
 * @brief addDeconvolution Добавляет анти-сверточный слой
 * @param name Имя слоя
 * @param nodes узлы с которыми связан слой (через пробел)
 * @param filters_ Количество нейронов
 * @param act_ функция активации
 * @param opt_ Оптимизатор
 * @param dropOut_
 * @param bnorm_
 * @param fWidth_
 * @param fHeight_
 * @param stride_
 * @param gpuDeviceId_ ID видеокарты для рсчетов
 * @return
 */
KERAS_EXPORT Status addDeconvolution(const char *name, const char *nodes, unsigned int filters_,
                                     Activation act_ = ACTIV_RELU,
                                     Optimizer opt_ = OPTIM_ADAM,
                                     float dropOut_ = 0.0,
                                     BatchNormType bnorm_ = BATCH_NONE,
                                     unsigned int fWidth_ = 3,
                                     unsigned int fHeight_ = 3,
                                     unsigned int stride_ = 2,
                                     unsigned int gpuDeviceId_ = 0);

/**
 * @brief addPooling Добавляет слой объединения
 * @param name Имя слоя
 * @param nodes узлы с которыми связан слой (через пробел)
 * @param kernel_
 * @param stride_
 * @param pool_ Типы объединения
 * @param gpuDeviceId_ ID видеокарты для рсчетов
 * @return Статус добавления слоя в модель
 */
KERAS_EXPORT Status addPooling(const char *name, const char *nodes, unsigned int kernel_,
                               unsigned int stride_,
                               PoolType pool_ = POOL_MAX,
                               unsigned int gpuDeviceId_ = 0);

/**
 * @brief addDense Добавляет плотный слой
 * @param name Имя слоя
 * @param nodes узлы с которыми связан слой (через пробел)
 * @param units_ Количество нейронов
 * @param act_ Функция активации
 * @param opt_ Функция оптимизации
 * @param dropOut_
 * @param bnorm_ Нормализация набора
 * @param gpuDeviceId_ ID устройства GPU
 * @return Статус добавления слоя в модель
 */
KERAS_EXPORT Status addDense(const char *name, const char *nodes, unsigned int units_,
                          Activation act_ = ACTIV_RELU,
                          Optimizer opt_ = OPTIM_ADAM,
                          float dropOut_ = 0.0,
                          BatchNormType bnorm_ = BATCH_NONE,
                          unsigned int gpuDeviceId_ = 0);

/**
 * @brief addConcat Слой объединения
 * @param name Имя слоя
 * @param nodes узлы с которыми связан слой (через пробел)
 * @param sequence Имена объединяемых слоев (через пробел)
 * @return Статус добавления слоя в модель
 */
KERAS_EXPORT Status addConcat(const char *name, const char *nodes, const char * sequence);

/**
 * @brief addResize Слой изменения размера
 * @param name Имя слоя
 * @param nodes узлы с которыми связан слой (через пробел)
 * @param fwdBegin Начальное значение перед
 * @param fwdEnd Конечное значение перед
 * @param bwdBegin Начальное значение зад
 * @param bwdEnd Конечное значение зад
 * @return Статус добавления слоя в модель
 */
KERAS_EXPORT Status addResize(const char *name, const char *nodes,
                              unsigned int fwdBegin, unsigned int fwdEnd,
                              unsigned int bwdBegin, unsigned int bwdEnd);

/**
 * @brief addCrop Отсечение каналов в изображении
 * @param name Имя слоя
 * @param nodes узлы с которыми связан слой (через пробел)
 * @param x координата
 * @param y координата
 * @param w длина
 * @param h ширина
 * @return Статус добавления слоя в модель
 */
KERAS_EXPORT Status addCrop(const char *name, const char *nodes, unsigned int x, unsigned int y,
                            unsigned int w, unsigned int h);

/**
 * @brief addSummator Добавляет слой суммирования
 * @param name Имя слоя
 * @param nodes узлы с которыми связан слой (через пробел)
 * @param type тип суммирования
 * @return Статус добавления слоя в модель
 */
KERAS_EXPORT Status addSummator(const char *name, const char *nodes, SummatorType type);

/**
 * @brief addActivator Добавляет слой с ф-ией активации
 * @param name Имя слоя
 * @param nodes узлы с которыми связан слой (через пробел)
 * @param active Функции активации
 * @return Статус добавления слоя в модель
 */
KERAS_EXPORT Status addActivator(const char *name, const char *nodes, Activation active);

/**
 * @brief addLossFunction Добавляет функцию потерь в модель
 * @param name Имя слоя
 * @param nodes  узли с которыми связан слой (через пробел)
 * @param loss_ тип функции потерь
 * @return Статус добавления слоя в модель
 */
KERAS_EXPORT Status addLossFunction(const char *name, const char *nodes, LossType loss_);

/**
 * @brief netArchitecture Записывает архитектуру сети в JSON виде
 * @param buffer Буфер для записи
 * @param length Размер буфера
 * @return Статус вывода архитектуры
 */
KERAS_EXPORT Status netArchitecture(char * buffer, unsigned int length);

/**
 * @brief fit Запуск упрощенной тренировки для известных наборов
 * @param data тренировочные данные
 * @param dataSize количество тренировочных данных
 * @param label метки тренировочных данных
 * @param labelsSize количество меток
 * @param epochs Эпохи
 * @param learningRate Шаг обучения
 * @return Статус тренировки
 */
KERAS_EXPORT Status fit(float * data, LayerSize dataSize, unsigned char * label,
                        LayerSize labelsSize, unsigned int epochs,
                        float learningRate);

/**
 * @brief lastAccurateSum Уровень обучения по результатам упрощенной тренировки
 * @return значение уровня обучения
 */
KERAS_EXPORT float lastAccurateSum();

/**
 * @brief trainCreate Запуск тренировки без эпох (пока не надоест)
 * @param data тренировочные данные
 * @param dataSize количество тренировочных данных
 * @param label метки тренировочных данных
 * @param labelsSize количество меток
 * @param learningRate Шаг обучения
 * @return Статус начала обучения
 */
KERAS_EXPORT Status trainCreate(float * data, LayerSize dataSize, unsigned char * label, LayerSize labelsSize);

/**
 * @brief trainStep Очередной шаг обучения
 * @return Статус шага обучения
 */
KERAS_EXPORT Status trainStep(float learningRate, LayerSize dataSize);

/**
 * @brief trainStop Остановка обучения и высвобождение памяти
 * @return Статус окончания обучения
 */
KERAS_EXPORT Status trainStop();

/**
 * @brief evaluate Проверка с тестовым сетом
 * @param data тестовые данные
 * @param dataSize количество тестовых данных
 * @param label метки тестовые данных
 * @param labelsSize количество меток
 * @param verbose уровень подробности
 * @return Статус тестирования
 */
KERAS_EXPORT Status evaluate(float * data, LayerSize dataSize, unsigned char * label,
                             LayerSize labelsSize, unsigned int verbose);

/**
 * 
 */
KERAS_EXPORT float testPercents();

/**
 * @brief saveModel Сохраняет модель с весами
 * @param filename имя с путем
 * @return Статус сохранения
 */
KERAS_EXPORT Status saveModel(const char * filename);

/**
 * @brief loadModel Загружает модель с весами
 * @param filename имя с путем
 * @return Статус загрузки
 */
KERAS_EXPORT Status loadModel(const char * filename);

/**
 * @brief lastError Вывод последней ошибки в буфер
 * @param buffer Буфер для текста ошибки
 * @param length размер буфера
 */
KERAS_EXPORT void lastError(char * buffer, unsigned int length);

/**
 * @brief printLastError Вывод последней ошибки в stdout
 */
KERAS_EXPORT void printLastError(Status);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif // KERAS_H
