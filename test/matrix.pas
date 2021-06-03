program matrix;

var
    row1, col1, row2, col2: integer;
    A, B: array [0..19, 0..19] of integer;
    C: array [0..19, 0..19] of longint;
    i, j, k: integer;

begin
    read(row1, col1);
    for i := 0 to row1-1 do
    begin
        for j := 0 to col1-1 do
            read(A[i][j]);
        readln();        
    end;

    read(row2, col2);
    for i := 0 to row2-1 do
    begin
        for j := 0 to col2-1 do
            read(B[i][j]);
        readln();        
    end;

    if col1 <> row2 then
        writeln('Incompatible Dimensions')
    else
    begin
        for i := 0 to row1-1 do
            for j := 0 to col2-1 do
                C[i, j] := 0;
        for i := 0 to row1-1 do
            for j := 0 to col2-1 do
                for k := 0 to row2-1 do
                    C[i, j] := C[i, j] + A[i, k] * B[k, j];

        for i := 0 to row1-1 do
        begin
            for j := 0 to col2-1 do
                write(C[i][j]:10);
            writeln();
        end;
    end;

end.