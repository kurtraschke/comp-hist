#ParseCH, take three
package ParseCH;
require Exporter;

@ISA = qw(Exporter);
@EXPORT = qw(parsech dodate);
@EXPORT_OK = qw(%name %year %month %day %type %status %ref %info @linkfrom @linkto @linkweight $minyear $maxyear);
$VERSION = 2.00;

sub parsech{
	s/\#.*//;
	if(/^(\S+)/){ $node = $1; }
	if(/^\bName: (.*)/){ $name{$node} = $1; }
	if(/^\b[Date|Founded].*(\d\d\d\d)-(\d\d)-(\d\d)/){$year{$node} = $1; $month{$node} = $2; $day{$node} = $3; }
	if(/^\b[Date|Founded].*(\d\d\d\d)-(\d\d)/){$year{$node} = $1; $month{$node} = $2; }
	if(/^\b[Date|Founded].*(\d\d\d\d)/){ $year{$node} = $1; }
	if(/^\bReference: (.*)/){ $ref{$node} = $1; }
	if(/^\bInfo: (.*)/){ $info{$node} = $1; }
	if(/^\bType: hardware/i){ $type{$node} = "hardware"; }
	if(/^\bType: OS/i){ $type{$node} = "os"; }
	if(/^\bType: language/i){ $type{$node} = "language"; }
	if(/.*Type: standard/i){ $type{$node} = "standard"; }
	if(/^\bType: other/i){ $type{$node} = "other"; } #You don't know about this line. Go away.
	if(/.*Type: company/i){ $type{$node} = "company"; }
	if(/^\bType: announcement/i){$type{$node} = "announce"; }

	if(/.*Status: released/i){ $status{$node} = "released"; }
	if(/^\bStatus: internal/i){ $status{$node} = "internal"; } 
	if(/^\bStatus: continual evolution/i){ $status{$node} = "conevol"; }
	if(/^\bStatus: prototype/i){ $status{$node} = "prototype"; }
	if(/^\bStatus: research/i){ $status{$node} = "research"; }

	if(/^\bAka (\S+)/) {
	    push(@linkfrom, $node);
	    push(@linkto, $1);
	    push(@linkweight, "aka");
	}
	if (/^\bSuccessor to (\S+)/) {
            push(@linkfrom, $node);
            push(@linkto, $1);
            push(@linkweight, "successor");
	}
	if (/^\bCode taken from (\S+)/) {
            push(@linkfrom, $node);
            push(@linkto, $1);
            push(@linkweight, "codetaken");
	}
	if (/^\bInfluenced by (\S+)/) {
            push(@linkfrom, $node);
            push(@linkto, $1);
            push(@linkweight, "influenced");
	}
	if(/^\bRuns on (\S+)/){
            push(@linkfrom, $node);
            push(@linkto, $1);
            push(@linkweight, "runson");
	}
	if(/^\bRan on (\S+)/){
            push(@linkfrom, $node);
            push(@linkto, $1);
            push(@linkweight, "ranon");
	}
	if(/^\bWritten in (\S+)/){
            push(@linkfrom, $node);
            push(@linkto, $1);
            push(@linkweight, "writtenin");
	 }
    
}

sub dodate {
    foreach $node (keys %year){
	if($year{$node} < $minyear){ $minyear = $year{$node}; }
	if($year{$node} > $maxyear){ $maxyear = $year{$node}; }
    }
}
