#ParseCH.pm, take 2
package ParseCH;
require Exporter;

@ISA = qw(Exporter);
@EXPORT = qw(docomment doname dodate doref doinfo dotype dostatus doaka dosucc doctf doinfluence doruns doran dowritten);
@EXPORT_OK = qw(dofield do1field donode %name %year %month %day %ref %info %type %status @linkfrom @linkto @weight);
$VERSION = 1.10;

sub docomment {
	s/#.*//;
}

sub dofield {
    donode;
    if(/^\b$_[0].? (.*)/){ my $field = $1; }
    return $field;
}

sub do1field {
    donode;
    if(/^\b$_[0]:? ?(\S+)/){ my $field = $1; }
    return $field;
}

sub donode {
    $node = do1field();
}

sub doname {
    $name{$node} = dofield(Name);
}

sub dodate {
    if(/^\b[Date|Founded].*(\d\d\d\d)-((\d\d)?-(\d\d)?)?/){
	$year{$node} = $1;
	$month{$node} = $3;
	$day{$node} = $4;
    }
}

sub doref {
    $ref{$node} = dofield(Reference);
}

sub doinfo {
    $info{$node} = dofield(Info);
}

sub dotype {
    $type{$node} = do1field(Type);
}

sub dostatus {
    $status{$node} = do1field(Status);
}

sub doaka {
    my $aka = do1field(Aka);
    push(@linkfrom, $node);
    push(@linkto, $ran);
    push(@weight, "runson");
}

sub dosucc {
    my $succ = do1field("Successor to");
    push(@linkfrom, $node);
    push(@linkto, $ran);
    push(@weight, "runson");
}

sub doctf {
    my $code = do1field("Code taken from");
    push(@linkfrom, $node);
    push(@linkto, $code);
    push(@weight, "codetaken");
}

sub doinfluence {
    my $inf = do1field("Influenced by");
    push(@linkfrom, $node);
    push(@linkto, $inf);
    push(@weight, "influenced");
}

sub doruns {
    my $runs = do1field("Runs on");
    push(@linkfrom, $node);
    push(@linkto, $runs);
    push(@weight, "runson");
}

sub doran {
    my $ran = do1field("Ran on");
    push(@linkfrom, $node);
    push(@linkto, $ran);
    push(@weight, "runson");
}

sub dowritten {
    my $written = do1field("Written in");
    push(@linkfrom, $node);
    push(@linkto, $ran);
    push(@weight, "runson");
}
