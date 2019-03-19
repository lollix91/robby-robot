% Logica di controllo esterna del robot

:-dynamic temp/1.
:-dynamic safetemp/1.
:-dynamic criticaltemp/1.


% regole per il ragionamento e la decisione
saveme(X, Y) :- temp(X), criticaltemp(X1), X > X1, Y = 1.
saveme(X, Y) :- temp(X), safetemp(X1), criticaltemp(X2), X > X1, X < X2, Y = 2.
