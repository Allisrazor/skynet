﻿unit USummatorLayer;

interface
uses Windows, Classes, DataTypes, SysUtils, RunObjts, UAbstractLayer;

type

  TSummatorLayer = class(TAbstractLayer)
  public
    // Конструктор класса
    constructor  Create(Owner: TObject); override;
    // Деструктор
    destructor   Destroy; override;
    function       InfoFunc(Action: integer;aParameter: NativeInt):NativeInt;override;
    function       RunFunc(var at,h : RealType;Action:Integer):NativeInt;override;
    function       GetParamID(const ParamName:string;var DataType:TDataType;var IsConst: boolean):NativeInt;override;
    // Добавляет данный слой в модель
    procedure addLayerToModel(id : Integer); override;
    // Функция для обеспечения изменения визуальных параметров блока
    procedure EditFunc(Props:TList;
                       SetPortCount:TSetPortCount;
                       SetCondPortCount:TSetCondPortCount;
                       ExecutePropScript:TExecutePropScript);override;

  private
    isCreate: Boolean;
    m_outputQty: NativeInt;// Количество связей с другими слоями
    m_sumType : Integer; // Тип суммирования
  const
    PortType = 0; // Тип создаваемых портов (под математическую связь)
  end;

implementation
uses keras;

constructor  TSummatorLayer.Create;
begin
  inherited;
  shortName := 'SUM' + IntToStr(getLayerNumber);
  isCreate := False;
end;

destructor   TSummatorLayer.Destroy;
begin
  inherited;

end;

function    TSummatorLayer.GetParamID;
begin
  Result:=inherited GetParamId(ParamName,DataType,IsConst);
  if Result = -1 then begin
    if StrEqu(ParamName,'output_qty') then begin
      Result:=NativeInt(@m_outputQty);
      DataType:=dtInteger;
      Exit;
    end else if StrEqu(ParamName,'sum_type') then begin
      Result:=NativeInt(@m_sumType);
      DataType:=dtInteger;
      Exit;
    end;
  end;
end;

procedure TSummatorLayer.addLayerToModel(id : Integer);
var
  returnCode: TStatus;
begin
  returnCode := addSummator(id, PAnsiChar(shortName),
                            PAnsiChar(nodes),
                            m_sumType);
  if returnCode <> STATUS_OK then begin
    ErrorEvent('Neural model not added activation layer', msError, VisualObject);
    Exit;
  end;
end;

//----- Редактирование свойств блока -----
procedure TSummatorLayer.EditFunc;
var
  InputPortsNmb, OutputPortsNmb: integer;
  MsgLength: Integer;
begin
  SetCondPortCount(VisualObject, m_outputQty - 1, pmOutput, PortType, sdRight, 'outport_1');
end;

function TSummatorLayer.InfoFunc(Action: integer;aParameter: NativeInt):NativeInt;
var
  I : Integer;
begin
  Result:=0;
  case Action of
    i_GetCount: begin
      for I := 0 to m_outputQty - 1 do
        cY[I] := 1;
      cY[0] := 3;
    end;
  else
    Result:=inherited InfoFunc(Action, aParameter);
  end
end;

function   TSummatorLayer.RunFunc;
var
  rootLayer: TAbstractLayer; // Родительский слой
  rootIndex: NativeInt;      // Индекс родительского слоя
  J : Integer;
begin
  Result:=0;
  case Action of
    f_UpdateOuts: begin

    end;
    f_InitState: begin
      nodes := '';
      isCreate := False;
    end;
    f_GoodStep: begin
//      if isCreate = False then begin
      if U[0].FCount > 0 then begin
        rootIndex := Round(U[0].Arr^[0]);
        if ((rootIndex >= 0) AND (rootIndex < LayersDict.Count)) then begin
          rootLayer := TAbstractLayer(LayersDict[rootIndex]);
          rootLayer.appendNode(shortName);
          for J := 0 to cY.Count - 1 do
            Y[J].Arr^[0] := getLayerNumber;
          isCreate := True;
        end;
        if U[0].FCount = 3 then begin
          Y[0].Arr^[1] := U[0].Arr^[1];
          Y[0].Arr^[2] := U[0].Arr^[2];
        end;
      end;
//      end;
    end;
  end
end;
end.
