#!/usr/bin/perl -w 
#Biblio.pl, written on a peice of paper on 8-7-0

while(<>){
    if(/^S+/){ $node = $1; next; };
    if(/.*Name: (.*)/){ $name{$node} = $1; };
    if(/.*Reference: (.*)/){ $ref{$node} = $1; };
}

foreach $node (%ref){
    print{"$name{$node}: $ref{$node}"};
}
