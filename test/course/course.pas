program course;

type
    base_char = array [0..100] of char;
    base_more = array [0..100] of base_char;
    base_int = array [0..100] of integer;
// All len* is (the length of *) - 1 except for len_name
var
    input: array [0..200] of base_char;
    total: integer;

    title: array [0..200] of base_char;
    len_title: array [0..200] of integer;
    credit: array [0..200] of integer;
    prereq_all: array [0..200] of base_char;
    prereq_each: array [0..100] of base_more; // 100 * 100 * 100
    len_prereq: array [0..200] of integer;
    grade: array [0..200] of char;

    GPA: real;
    attempt_credit: integer;
    complet_credit: integer;
    remain_credit: integer;
    total_grade: real;
    grade_temp: integer;

    len_group: integer; // how many groups of each prereq_all
    len_ind: array [0..100] of integer; // how many titles in each group
    len_name: array [0..100] of base_int; // 100 * 100, how long is each name(title)

    i: integer;
    j: integer;
    k: integer;
    l: integer;
    m: integer;
    acc: integer;
    flag1: integer; // whether to iterate in the same title
    flag2: integer; // whether to iterate in all the title
    flag3: integer; // not exist or not taken
    flag4: integer; // whether to iterate in the same group
    flag5: integer; // whether to iterate in all groups

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
        // title
        while input[total][i] <> '|' do
        begin
            title[total][len_title[total]] := input[total][i];
            len_title[total] := len_title[total] + 1;
            i := i + 1;
        end;
        len_title[total] := len_title[total] - 1;

        i := i + 1;
        // credit
        credit[total] := Ord(input[total][i]) - 48;
        i := i + 2;

        // prereq_all
        while input[total][i] <> '|' do
        begin
            prereq_all[total][len_prereq[total]] := input[total][i];
            len_prereq[total] := len_prereq[total] + 1;
            i := i + 1;
        end; 
        len_prereq[total] := len_prereq[total] - 1;

        i := i + 1;
        // grade
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
    total := total - 1;
    if total_grade = 0.0 then
        GPA := 0.0
    else
        GPA := total_grade / attempt_credit;

    write('GPA: ');
    writeln(GPA:3:1); // watch out
    write('Hours Attempted: ');
    writeln(attempt_credit);
    write('Hours Completed: ');
    writeln(complet_credit);
    write('Credits Remaining: ');
    writeln(remain_credit);
    writeln();
    writeln('Possible Courses to Take Next');
    
    if remain_credit = 0 then
        writeln('  None - Congratulations!')
    else
    begin
    for i := 0 to total do
    begin
        if (Ord(grade[i]) = 0) or (grade[i] = 'F') then
        begin
            if Ord(prereq_all[i][0]) = 0 then
            begin
                write('  ');
                writeln(title[i]);                        
            end
            else
            begin
                // initialize len
                len_group := 0;
                for j := 0 to 100 do
                begin
                    len_ind[j] := 0;
                    for k := 0 to 100 do
                    begin
                        len_name[j][k] := 0;
                        for l := 0 to 100 do
                            prereq_each[j][k][l] := chr(0);
                    end;    
                end;

                // acquire prereq_each
                for j := 0 to len_prereq[i] do
                begin
                    if prereq_all[i][j] = ',' then
                    begin
                        len_ind[len_group] := len_ind[len_group] + 1;
                    end
                    else if prereq_all[i][j] = ';' then
                    begin
                        len_group := len_group + 1;
                    end
                    else
                    begin
                        prereq_each[len_group][len_ind[len_group]][len_name[len_group][len_ind[len_group]]] := prereq_all[i][j];
                        len_name[len_group][len_ind[len_group]] := len_name[len_group][len_ind[len_group]] + 1;
                    end;
                end;

                // find prereq_each in all the titles
                l := 0;
                flag5 := 0;
                // iterate in all groups to get a group
                while (l < len_group + 1) and (flag5 = 0) do
                begin
                    m := 0;
                    flag4 := 0;
                    flag3 := 0;
                    acc := 0;
                    // iterate in a single group to get a prereq_each
                    while (m < len_ind[l] + 1) and (flag4 = 0) and (flag3 = 0) do
                    begin
                        j := 0;
                        flag2 := 0;
                        // iterate in all titles to get a title
                        while (j < total + 1) and (flag2 = 0) do
                        begin
                            k := 0;
                            flag1 := 0;
                            // iterate in a title to tell if match
                            while (k < len_name[l][m]) and (flag1 = 0) do
                            begin
                                if title[j][k] <> prereq_each[l][m][k] then
                                    flag1 := 1;
                                k := k + 1;
                            end;
                            // prereq_each matches the title
                            if k = len_name[l][m] then
                                flag2 := 1;
                            j := j + 1;
                        end;
                        // no match
                        if flag2 = 0 then
                            flag3 := 1
                        else
                        begin
                            j := j - 1;
                            if (Ord(grade[j]) <> 0) and (grade[j] <> 'F') then
                            begin
                                acc := acc + 1;
                                // all individuals in a group have passed
                                if acc = len_ind[l] + 1 then
                                    flag4 := 1;
                            end;
                        end;
                        m := m + 1;
                    end;
                    if flag4 = 1 then
                    begin
                        write('  ');
                        writeln(title[i]);
                        flag5 := 1;
                    end;
                    l := l + 1;
                end;
            end;
        end;
    end;
    end;
end.