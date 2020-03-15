//**************************************************************************//
 // Данный исходный код является составной частью системы             //
 // Программист:        Никишин Е. В.                                        //
 //**************************************************************************//

unit UDataSet;

interface

uses Windows, Classes, DataTypes, SysUtils, RunObjts, dataset;

type

  TDataSet = class(TRunObject)
  public
    // Конструктор класса
    constructor  Create(Owner: TObject); override;
    // Деструктор
    destructor   Destroy; override;
    function       InfoFunc(Action: integer;aParameter: NativeInt):NativeInt;override;
    function       RunFunc(var at,h : RealType;Action:Integer):NativeInt;override;
    function       GetParamID(const ParamName:string;var DataType:TDataType;var IsConst: boolean):NativeInt;override;

  strict private
     m_stepNumber: Cardinal;
     m_width: Integer;  /// Ширина одного набора данных
     m_height: Integer; /// Высота одного набора данных
     m_depth: Integer;  /// Глубина одного набора данных
     m_id: Integer;

     m_datasetType: NativeInt;  /// Тип набора
     m_trainData: String; /// Расположение тренировочных данных
     m_trainlabel: String;  /// Расположение тренировочных меток
     m_testData: String;  /// Расположение тестовых данных
     m_testLabel: String; /// Расположение тестовых меток
//     m_mnistData: TMNIST_DATA; /// Тренировочные данные о MNIST
     m_trainQty : Cardinal; /// Количество считываемых данны
     m_sendDataType : Cardinal; /// Тип отправляемых данных
  end;

implementation

uses keras, UNNConstants;

constructor  TDataSet.Create;
begin
  inherited;
  m_stepNumber:= 0;
  m_width := 1;
  m_height := 1;
  m_depth := 1;
  m_id := -1;
end;

destructor   TDataSet.Destroy;
begin
  inherited;
  if m_id >= 0 then begin
    dataset.deleteMnistDataset(m_id);
  end;
end;

function    TDataSet.GetParamID;
begin
  Result:=inherited GetParamId(ParamName,DataType,IsConst);
  if Result = -1 then begin
     if StrEqu(ParamName,'TrainData') then begin
      Result:=NativeInt(@m_trainData);
      DataType:=dtString;
      Exit;
    end else if StrEqu(ParamName,'TrainLabel') then begin
      Result:=NativeInt(@m_trainlabel);
      DataType:=dtString;
      Exit;
    end else if StrEqu(ParamName,'TestData') then begin
      Result:=NativeInt(@m_testData);
      DataType:=dtString;
      Exit;
    end else if StrEqu(ParamName,'TestLabel') then begin
      Result:=NativeInt(@m_testLabel);
      DataType:=dtString;
      Exit;
    end else if StrEqu(ParamName,'dataset_type') then begin
      Result:=NativeInt(@m_datasetType);
      DataType:=dtInteger;
      Exit;
    end else if StrEqu(ParamName,'train_qty') then begin
      Result:=NativeInt(@m_trainQty);
      DataType:=dtInteger;
      Exit;
    end else if StrEqu(ParamName,'send_data_type') then begin
      Result:=NativeInt(@m_sendDataType);
      DataType:=dtInteger;
      Exit;
    end;
  end
end;

function TDataSet.InfoFunc(Action: integer;aParameter: NativeInt):NativeInt;
begin
  Result:=0;
  case Action of
    i_GetCount: begin
      if m_datasetType = 0 then begin
        m_width := 28;
        m_height := 28;
        m_depth := 1;
      end;
      cY[0] := m_trainQty * m_width * m_height * m_depth; // Количество данных = количеству наборов * W * H
      cY[1] := m_trainQty; // Количество меток = количеству наборов
    end;
  else
    Result:=inherited InfoFunc(Action,aParameter);
  end
end;

function   TDataSet.RunFunc;
var
  returnCode: TStatus;
//  p64: UInt64;
  i, j: Integer;
  isWorkDone: Boolean;
  datas, labels : String;
  dataPoint : array of Single;
  labelPoint: array of Byte;
  parameters: TMNIST_DATA;
begin
  Result:=0;
  isWorkDone := True;
  case Action of
    f_UpdateOuts: begin

    end;
    f_InitState: begin
      m_stepNumber := 0;
//      m_mnistData.quantity := 0;
      // Зануление выходных портов
      for I := 0 to cY.Count - 1 do begin
        cY.Arr^[I] := 0;
        for J := 0 to Y[I].Count - 1 do  begin
          Y[I].Arr^[J] := 0;
        end;
      end;
      if m_id >= 0 then begin
        dataset.deleteMnistDataset(m_id);
      end;
      if m_sendDataType = 0  then begin
        datas := m_trainData;
        labels := m_trainLabel;
      end else begin
        datas := m_testData;
        labels := m_testLabel;
      end;
      if FileExists(datas) AND FileExists(labels) then begin
        m_id := dataset.createMnistDataset( PAnsiChar(AnsiString(datas)),
                                            PAnsiChar(AnsiString(labels)) );
      end else begin
        ErrorEvent('MNIST files not exist!', msError, VisualObject);
        Exit;
      end;

//      if m_isOneSend = True then begin // Только для пакета MNIST
        // Считываем тренировочные данные
//        if m_sendDataType = 0  then begin
//          datas := m_trainData;
//          labels := m_trainLabel;
//        end else begin
//          datas := m_testData;
//          labels := m_testLabel;
//        end;
//        // Проверяем доступность файлов
//        if FileExists(datas) AND FileExists(labels) then begin
//          returnCode := dataset.readMnistTrain(PAnsiChar(AnsiString(datas)),
//                                               PAnsiChar(AnsiString(labels)),
//                                               m_trainQty);
//          if returnCode <> STATUS_OK then begin
//            ErrorEvent('Read MNIST db is failure!', msError, VisualObject);
//            Exit;
//          end;
//          m_mnistData := mnistTrainParams;
//          ErrorEvent('Read MNIST: ' + IntToStr(m_mnistData.quantity), msInfo, VisualObject);
//        end else begin
//          ErrorEvent('Read MNIST failure ', msError, VisualObject);
//          isWorkDone := False;
//          Exit;
//        end;
//      end;
    end;
    f_GoodStep: begin
      if m_sendDataType = 0  then begin
        datas := m_trainData;
        labels := m_trainLabel;
      end else begin
        datas := m_testData;
        labels := m_testLabel;
      end;
      if FileExists(datas) AND FileExists(labels) then begin
        parameters := dataset.mnistParameters(m_id);
        SetLength(dataPoint, m_trainQty * m_width * m_height * m_depth);
        SetLength(labelPoint,m_trainQty);
        returnCode := dataset.readMnist(m_id, @dataPoint[0], @labelPoint[0],
                                        m_trainQty, m_stepNumber);
        if returnCode <> STATUS_OK then begin
          ErrorEvent('Read MNIST db is failure!', msError, VisualObject);
          Exit;
        end;
        for I := 0 to Length(dataPoint) - 1 do
          Y[0].Arr^[I] := dataPoint[I];
        for I := 0 to Length(labelPoint) - 1 do
          Y[1].Arr^[I] := labelPoint[I];
      end else begin
        ErrorEvent('MNIST files not exist!', msError, VisualObject);
        Exit;
      end;
      //----- Только для одноразовой посылки всех данных -----
//      if m_stepNumber = 0 then
//      begin
//        if (m_isOneSend = True) AND (isWorkDone = True) then begin // Только для пакета MNIST
//          Y[0].Arr^[0] := UNN_DATASEMNIST;
//          p64 := UInt64(@m_mnistData);  // Для тренировочных данных
//          Y[0].Arr^[1] := p64 shr 32;
//          Y[0].Arr^[2]:= (p64 shl 32) shr 32;
//        end else begin // Если данные будут идти по шагам
//          Y[0].Arr^[0] := UNN_DATASTEPBYSTEP;
//          Y[0].Arr^[1] := 0;
//          Y[0].Arr^[2]:= 0;
//        end;
      //----- Для посылки по шагам -----
//      end else begin
//        if m_isOneSend = True then begin
//          Y[0].Arr^[0] := UNN_DATASEMNIST;
//          Y[0].Arr^[1] := 0;
//          Y[0].Arr^[2]:= 0;
//        end else begin
//          // Считываем тренировочные данные
//          if m_sendDataType = 0  then begin
//            datas := m_trainData;
//            labels := m_trainLabel;
//          end else begin
//            datas := m_testData;
//            labels := m_testLabel;
//          end;
//          // Проверяем доступность файлов
//          if FileExists(datas) AND FileExists(labels) then begin
//            returnCode := dataset.readMnistTrain(PAnsiChar(AnsiString(datas)),
//                                                 PAnsiChar(AnsiString(labels)),
//                                                 m_trainQty, m_stepNumber);
//            if returnCode <> STATUS_OK then begin
//              ErrorEvent('Read MNIST db is failure!', msError, VisualObject);
//              Exit;
//            end;
//            m_mnistData := mnistTrainParams;
//            Y[0].Arr^[0] := UNN_DATASTEPBYSTEP;
//            p64 := UInt64(@m_mnistData);  // Для тренировочных данных
//            Y[0].Arr^[1] := p64 shr 32;
//            Y[0].Arr^[2]:= (p64 shl 32) shr 32;
//          end else begin
//            ErrorEvent('Read MNIST failure ', msError, VisualObject);
//            Exit;
//          end;
//        end;
//      end;
      inc(m_stepNumber);
    end;
  end
end;
end.
