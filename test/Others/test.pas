program test;

var 
    s1 : string;
    s2 : string;
    s3 : string;
    s4 : string;
    i1 : integer;
    i2 : integer;
    i3 : integer;
    c1 : char;

function f1():integer;
    function f2():integer;
    begin
        f2 := 4;
    end;
begin
    f1 := f2() + 1;
end;

begin
    // system function
    s1 := 'Hello World!';
    s2 := 'Compiler Project.';
    s3 := 'That is it.';
    s4 := '1234';
    i1 := 2;
    i2 := 100;
    i3 := -1000;
    c1 := 'c';
    writeln(concat(s1, s2));
    writeln(val(s4));
    writeln(str(i2));
    writeln(length(s4));
    writeln(succ(c1));
    writeln(sqr(i1));
    writeln(sqrt(i2));
    writeln(abs(i3));
    // string
    writeln(s1[0]);
    // subfunction
    writeln(f1());
end.


