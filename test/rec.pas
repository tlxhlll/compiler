program sysfunc;

type
  rec = record
    ia: integer;
    ic: string;
    ie: real;
  end;
var
  r: rec;

procedure test(re: rec);
begin
  readln(re.ia);
  readln(re.ic);
  readln(re.ie);

  writeln(re.ia);
  writeln(re.ic);
  writeln(re.ie);
end;
begin test(r); end.
