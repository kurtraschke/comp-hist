$minyear=3000;
$maxyear=0;
$linkcnt=0;
while (<>) {
    if (/^\#/) { next; }
    if (/^(\S+)/) { $node=$1; next; }
    if (/.*Name: (.*)/) { $Name{$node}=$1; $nodecnt++; next; }

    if (/.*Successor to (\S+)/) {
        $From[$linkcnt]=$1; $To[$linkcnt]=$node; $Link[$linkcnt]="succ";
        $linkcnt++;
    }
    if (/.*Code taken from (\S+)/) {
        $From[$linkcnt]=$1; $To[$linkcnt]=$node; $Link[$linkcnt]="code";
        $linkcnt++;
    }
    if (/.*Influenced by (\S+)/) {
        $From[$linkcnt]=$1; $To[$linkcnt]=$node; $Link[$linkcnt]="idea";
        $linkcnt++;
    }

    if (/.*Type: hardware/){ $color{$node} = yellow; next; }
    if (/.*Type: [OS|os]/){ $color{$node} = blue; next; }
    if (/.*Type: standard/){ $color{$node} = red; next; }
    if (/.*Type: language/){ $color{$node} = green; next; }


    if (/.*Date:.*(\d\d\d\d)-(\d\d)-(\d\d)/) {
                            # Convert to year-mon
        $yr=$1; $mon=$2; goto dodate;
    }
    if (/.*Date:.*(\d\d\d\d)-(\d\d)/) {
        $yr=$1; $mon=$2; goto dodate;
    }
    if (/.*Date:.*(\d\d\d\d)/) {
        $yr=$1; $mon="00";
dodate:
        $d="$yr.$mon"; $Date{$d}{$node}=$node;
        $Date{$d}{$d}=$d; $Name{$d}=$d;
        if ($yr > $maxyear) { $maxyear=$yr; }
        if ($yr < $minyear) { $minyear=$yr; }
        next;
	}
}
