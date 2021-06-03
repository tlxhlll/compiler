program course;

var
    input: array [0..200, 0..200] of char;
    total: integer;

    title: array [0..200, 0..100] of char;
    len_title: array [0..200] of integer;
    credit: array [0..200] of integer;
    prereq: array [0..200, 0..100] of char;
    len_prereq: array [0..200] of integer;
    grade: array [0..200] of char;

    GPA: real;
    attempt_credit: integer;
    complet_credit: integer;
    remain_credit: integer;
    total_grade: real;
    grade_temp: integer;

    i: integer;
    j: integer;
    k: integer;
begin
    total := 0;
    attempt_credit := 0;
    complet_credit := 0;
    remain_credit := 0;
    total_grade := 0.0;
    grade_temp := 0;

    readln(input[total]);
    while Ord(input[total][0]) <> 0 do
    begin
        len_title[total] := 0;
        len_prereq[total] := 0;
        i := 0;
        while input[total][i] <> '\0' do
        begin
            if input[total][i] <> '|' then
            begin
                title[total][len_title[total]] := input[total][i];
                len_title[total] := len_title[total] + 1;
            end
            else
            begin
                i := i + 1;
                break;
            end;    
            i := i + 1;
        end; 

        credit[total] := Ord(input[total][i]) - 48;
        i := i + 2;

        while input[total][i] <> '\0' do
        begin
            if input[total][i] <> '|' then
            begin
                prereq[total][len_prereq[total]] := input[total][i];
                len_prereq[total] := len_prereq[total] + 1;
            end
            else
            begin
                i := i + 1;
                break;
            end;    
            i := i + 1;
        end; 
        

        grade[total] := input[total][i];
        if Ord(grade[total]) <> 0 then
            attempt_credit := attempt_credit + credit[total];

        if (Ord(grade[total]) <> 0) and (grade[total] <> 'F') then
        begin
            complet_credit := complet_credit + credit[total];
            if grade[total] = 'A' then
                grade_temp := 4;
            if grade[total] = 'B' then
                grade_temp := 3;
            if grade[total] = 'C' then
                grade_temp := 2;
            if grade[total] = 'D' then
                grade_temp := 1;
            total_grade := total_grade + credit[total] * grade_temp;
        end;
            
        if (Ord(grade[total]) = 0) or (grade[total] = 'F') then
            remain_credit := remain_credit + credit[total];
        
        total := total + 1;
        readln(input[total]);
    end;

    if total_grade = 0 then
        GPA := 0.0
    else
        GPA := total_grade / attempt_credit;;

    write('GPA: ');
    writeln(GPA:3:1);
    write('Hours Attempted: ');
    writeln(attempt_credit);
    write('Hours Completed: ');
    writeln(complet_credit);
    write('Credits Remaining: ');
    writeln(remain_credit);
    writeln();
    writeln('Possible Courses to Take Next');

    for i := 0 to total-1 do
    begin
        if (Ord(grade[i]) = 0) or (grade[i] = 'F') then
        begin
            if Ord(prereq[i][0]) = 0 then
            begin
                write('  ');
                writeln(title[i]);                        
            end
            else
            begin
                for j := 0 to total-1 do
                begin
                    for k := 0 to len_prereq[i]-1 do
                        if title[j][k] <> prereq[i][k] then
                            break;
                    if k = len_prereq[i]-1 then
                        break;
                end;
                if (Ord(grade[j]) <> 0) and (grade[j] <> 'F') then
                begin
                    write('  ');
                    writeln(title[i]);
                end;
            end;
        end;
    end;

end.