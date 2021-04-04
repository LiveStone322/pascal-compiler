program posneg;
var 
 no : integer;
begin
  write('Enger a number:');
  readln(no);

  if (no > 0) then
   writeln('You enter Positive Number')
  else
    if (no < 0) then
     writeln('You enter Negative number')
    else
      if (no = 0) then
      writeln('You enter Zero');

  readln;
end.
