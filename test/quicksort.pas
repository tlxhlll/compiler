program quicksort;

var
    N: integer;
    arr: array [0..10000] of integer;
    i: integer;

procedure sort(front: integer; back: integer);
var
    pivotValue: integer;
    pivotPosition: integer;
    pos: integer;
    temp: integer;
begin
    if front < back then
    begin
        pivotValue := arr[front];
        pivotPosition := front;
        for pos := front+1 to back do
            begin
                if arr[pos] < pivotValue then
                begin
                    temp := arr[pos];
                    arr[pos] := arr[pivotPosition+1];
                    arr[pivotPosition+1] := temp;
                    
                    temp := arr[pivotPosition+1];
                    arr[pivotPosition+1] := arr[pivotPosition];
                    arr[pivotPosition] := temp;

                    pivotPosition := pivotPosition + 1;
                end; 
            end;
        sort(front, pivotPosition - 1);
        sort(pivotPosition + 1, back);
    end;
end;

begin
    readln(N);
    for i := 0 to N-1 do
        readln(arr[i]);
    sort(0, N-1);

    for i := 0 to N-1 do
        writeln(arr[i]);
end.


