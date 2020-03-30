﻿unit UConcatLayer;

interface
uses Windows, Classes, DataTypes, SysUtils, RunObjts, UAbstractLayer;

type

  TConcatLayer = class(TAbstractLayer)
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
    m_inputQty:  NativeInt;// Количество объединяемых слоев
    m_ccNodes: AnsiString; // Объединяемые ноды
  const
    PortType = 0; // Тип создаваемых портов (под математическую связь)
  end;

implementation
  uses keras;

constructor  TConcatLayer.Create;
begin
  inherited;
  shortName := 'CC' + IntToStr(getLayerNumber);
  isCreate := False;
end;

destructor   TConcatLayer.Destroy;
begin
  inherited;

end;

function    TConcatLayer.GetParamID;
begin
  Result:=inherited GetParamId(ParamName,DataType,IsConst);
  if Result = -1 then begin
    if StrEqu(ParamName,'output_qty') then begin
      Result:=NativeInt(@m_outputQty);
      DataType:=dtInteger;
      Exit;
    end;
  end;
end;

procedure TConcatLayer.addLayerToModel(id : Integer);
var
  returnCode: TStatus;
begin
  if id = m_modelID then begin
    returnCode := addConcat(id, PAnsiChar(shortName),
                            PAnsiChar(nodes),
                            PAnsiChar(m_ccNodes));
    if returnCode <> STATUS_OK then begin
      ErrorEvent('Neural model not added dense layer', msError, VisualObject);
      Exit;
    end;
  end;
end;

//----- Редактирование свойств блока -----
procedure TConcatLayer.EditFunc;
var
  InputPortsNmb, OutputPortsNmb: integer;
  MsgLength: Integer;
begin
  SetCondPortCount(VisualObject, m_inputQty - 1,  pmInput,  PortType, sdLeft,  'inport_1');
  SetCondPortCount(VisualObject, m_outputQty - 1, pmOutput, PortType, sdRight, 'outport_1');
end;

function TConcatLayer.InfoFunc(Action: integer;aParameter: NativeInt):NativeInt;
var
  I : Integer;
begin
  Result:=0;
  case Action of
    i_GetCount: begin
      for I := 0 to m_outputQty - 1 do
        cY[I] := 2;
      cY[0] := 4;
    end;
  else
    Result:=inherited InfoFunc(Action, aParameter);
  end
end;

function   TConcatLayer.RunFunc;
var
  rootLayer: TAbstractLayer; // Родительский слой
  rootIndex: NativeInt;      // Индекс родительского слоя
  I, J : Integer;
begin
  Result:=0;
  case Action of
    f_UpdateOuts: begin

    end;
    f_InitState: begin
      nodes := '';
      m_ccNodes := '';
      isCreate := False;
    end;
    f_GoodStep: begin
//      if isCreate = False then begin
      for I := 0 to cU.FCount - 1 do begin
        m_modelID := Round(U[I].Arr^[0]);
        rootIndex := Round(U[I].Arr^[1]);
        if ((rootIndex >= 0) AND (rootIndex < LayersDict.Count)) then begin
          rootLayer := TAbstractLayer(LayersDict[rootIndex]);
          rootLayer.appendNode(shortName);
          if Length(m_ccNodes) > 0 then begin
            m_ccNodes := m_ccNodes + ' ' + rootLayer.getShortName;
          end else begin
            m_ccNodes := rootLayer.getShortName;
          end;
          for J := 0 to cY.Count - 1 do begin
            Y[J].Arr^[0] := m_modelID;
            Y[J].Arr^[1] := getLayerNumber;
          end;
          isCreate := True;
        end;
        if U[0].FCount = 4 then begin
          Y[0].Arr^[2] := U[0].Arr^[2];
          Y[0].Arr^[3] := U[0].Arr^[3];
        end;
      end;
//      end;
    end;
  end
end;
end.
