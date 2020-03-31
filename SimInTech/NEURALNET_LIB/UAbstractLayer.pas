﻿unit UAbstractLayer;

interface
  uses
    Windows,
    Classes,
    DataTypes,
    SysUtils,
    RunObjts,
    Generics.Collections;

type

  TAbstractLayer = class(TRunObject)
  public
    constructor Create(Owner: TObject); override;
    function getLayerNumber(): NativeInt;
    function getShortName(): String;
    // Добавляет данный слой в модель
    procedure addLayerToModel(id : Integer); virtual; abstract;

    // Добавляет очередную ноду связи к слою
    procedure appendNode(nodeName: String);

  protected
   shortName: AnsiString;
   nodes: AnsiString;
   m_modelID : Integer;

  private
    layerNumber: NativeInt;

  end;
  PAbstractLayer = ^TAbstractLayer;

var
  LayersDict: TList<TObject>;
  LayerCount: NativeInt;

implementation

constructor  TAbstractLayer.Create;
begin
  inherited;
  layerNumber:= LayerCount;
  inc(LayerCount);
  LayersDict.Add(Self);
  nodes := '';
  m_modelID := -1;
end;

function TAbstractLayer.getLayerNumber(): NativeInt;
begin
  Result:= layerNumber;
end;

function TAbstractLayer.getShortName : String;
begin
  Result:= shortName;
end;

procedure TAbstractLayer.appendNode(nodeName: String);
begin
  if Length(nodes) > 0 then begin
    nodes := nodes + ' ' + nodeName;
  end else begin
    nodes := nodeName;
  end;
end;

initialization
  LayerCount:= 0;
  LayersDict := TList<TObject>.Create;

end.
