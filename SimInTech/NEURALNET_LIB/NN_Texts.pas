unit NN_Texts;


interface

const

  {$IFNDEF ENG}

  txtNN_ModelNotAdded       = '���������� �������� ���� � ������: ';
  txtNN_NCreated = '��������� ������ �� ���� �������';
  txtNN_TestCrash = '������� ������������ ���������� �������';
  txtNN_WeightLoad = '�������� ����� ��� ������ ����������� ������';
  txtNN_WeightSave = '���������� ����� ��� ������ ����������� ������';
  txtNN_WeightOpen = '���������� ������� ���� � ������ ������';
  txtNN_DataSize = '������������ �������� ��������� ������ � ��������� ��������� ������';
  txtDS_NotLoaded = '����� �������� ����� �� ��� ��������';
  {$ELSE}

  txtNN_ModelNotAdded       = 'Neural model not added layer: ';
  txtNN_NCreated = 'Neural model not created';
  txtNN_TestCrash = 'Test process is crashed';
  txtNN_WeightLoad = 'Crashed load model weight';
  txtNN_WeightSave = 'Crashed save model weight';
  txtNN_WeightOpen = 'Open weight file is crashed';
  txtNN_DataSize = 'Data size more Output size!';
  txtDS_NotLoaded = 'Test set not loaded';
  {$ENDIF}

implementation

end.
