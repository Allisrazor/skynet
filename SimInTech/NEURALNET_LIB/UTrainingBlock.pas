﻿unit UTrainingBlock;

interface

uses Windows, Classes, DataTypes, SysUtils, RunObjts, dataset;

type
  TTrainingBlock = class(TRunObject)
  public
    // Конструктор класса
    constructor  Create(Owner: TObject); override;
    // Деструктор
    destructor   Destroy; override;
    function       InfoFunc(Action: integer;aParameter: NativeInt):NativeInt;override;
    function       RunFunc(var at,h : RealType;Action:Integer):NativeInt;override;
    function       GetParamID(const ParamName:string;var DataType:TDataType;var IsConst: boolean):NativeInt;override;

  strict private
    stepCount: NativeInt; // Счетчик шагов
    m_nnState: Boolean; // Состояние сети
    m_trainData : pMNIST_DATA;
    m_testData : pMNIST_DATA;

    m_crossOut: NativeInt;
    m_learningRate : double;
    m_fileSave : String;
    m_fileLoad: String;

    m_label: array of Byte;

    m_lastDataType : Integer;

    m_id : Integer;
  end;

implementation
  uses keras, UNNConstants;

constructor  TTrainingBlock.Create;
begin
  inherited;
end;

destructor   TTrainingBlock.Destroy;
begin
  inherited;
end;

function    TTrainingBlock.GetParamID;
begin
  Result:=inherited GetParamId(ParamName,DataType,IsConst);
  if Result = -1 then begin
    if StrEqu(ParamName,'cross_out') then begin
      Result:=NativeInt(@m_crossOut);
      DataType:=dtInteger;
      Exit;
    end else if StrEqu(ParamName,'learning_rate') then begin
      Result:=NativeInt(@m_learningRate);
      DataType:=dtDouble;
      Exit;
    end else if StrEqu(ParamName,'file_save') then begin
      Result:=NativeInt(@m_fileSave);
      DataType:=dtString;
      Exit;
    end else if StrEqu(ParamName,'file_load') then begin
      Result:=NativeInt(@m_fileLoad);
      DataType:=dtString;
      Exit;
    end;
  end
end;

function TTrainingBlock.InfoFunc(Action: integer;aParameter: NativeInt):NativeInt;
begin
  Result:=0;
  case Action of
    i_GetCount: begin
      cY[0] := 1;
    end;
  else
    Result:=inherited InfoFunc(Action,aParameter);
  end
end;

function   TTrainingBlock.RunFunc;
var
  i, val : NativeInt;
  p64: UInt64;
  accuracy : Single;
  m_data: PDataArr; /// Данные, которые проходят через слои модели

  datas, labels : TLayerSize;
  returnCode : NativeInt;
begin
  Result:=0;
  case Action of
    f_UpdateOuts: begin

    end;
    f_InitState: begin
      stepCount := 0;
      m_lastDataType := 0;
    end;
    f_GoodStep: begin
//      if stepCount = 0 then // Только для первого шага
//      begin
        // Вход 0 - данные
        // Вход 1 - метки
      if cU.FCount <> 5 then begin
        ErrorEvent('Output ports qty != 5', msError, VisualObject);
        Exit;
      end;
      m_id := Round(U[0].Arr^[0]);
      p64 := Round(U[0].Arr^[1]);
      p64 := p64 shl 32;
      p64 := p64 OR UInt64(Round(U[0].Arr^[2]));
      m_data := PDataArr(p64);
      SetLength(m_label, U[1].Count);
      for I := 0 to Length(m_label) - 1 do begin
        m_label[I] := Round(U[1].Arr^[I]);
      end;
      datas.w := Round(U[2].Arr^[0]);
      datas.h := Round(U[3].Arr^[0]);
      datas.ch := Round(U[4].Arr^[0]);
      datas.bsz := Length(m_label);
      labels.w := m_crossOut;
      labels.h := 1;
      labels.ch := 1;
      labels.bsz := Length(m_label);
      fit(m_id, @m_data^[0], datas, @m_label[0], labels, 1, m_learningRate, accuracy);
      Y[0].Arr^[0] := accuracy;
      inc(stepCount);
    end;
    f_Stop : begin
      if m_fileSave.Length > 0 then begin
        returnCode := saveModel(m_id, PAnsiChar(AnsiString(m_fileSave)));
        if returnCode <> STATUS_OK then begin
          ErrorEvent('Crashed save model weight', msError, VisualObject);
          Exit;
        end;
      end;
    end;
  end;
end;
end.
