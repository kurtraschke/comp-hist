#ParseCH.pm, take 2
package ParseCH;
require Exporter;

@ISA = qw(Exporter);
@EXPORT = qw(docomment donode doname dodate doref doinfo dotype dostatus doaka dosucc doctf doinfluence doruns doran dowritten);
@EXPORT_OK = qw(dofield do1field);
$VERSION = 1.00;

sub docomment {
	s/#.*//;
}

sub dofield {
    if(/^\b$_[0].? (.*)/){ my $field = $1; }
    return $field;
}

sub do1field {
    if(/^\b$_[0]:? ?(\S+)/){ my $field = $1; }
    return $field;
}

sub donode {
    my $node = do1field();
    return $node;
}

sub doname {
    my $name = dofield(Name);
    return $name;
}

sub dodate {
    if(/^\b[Date|Founded].*(\d\d\d\d)-((\d\d)?-(\d\d)?)?/){
	my $year = $1;
	my $month = $3;
	my $day = $4;
    }
    return ($year, $month, $day);
}

sub doref {
    my $ref = dofield(Reference);
    return $ref;
}

sub doinfo {
    my $info = dofield(Info);
    return $info;
}

sub dotype {
    my $type = do1field(Type);
    return $type;
}

sub dostatus {
    my $status = do1field(Status);
    return $status;
}

sub doaka {
    my $aka = do1field(Aka);
    return $aka;
}

sub dosucc {
    my $succ = do1field("Successor to");
    return $succ;
}

sub doctf {
    my $code = do1field("Code taken from");
    return $code;
}

sub doinfluence {
    my $inf = do1field("Influenced by");
    return $inf;
}

sub doruns {
    my $runs = do1field("Runs on");
    return $runs;
}

sub doran {
    my $ran = do1field("Ran on");
    return $ran;
}

sub dowritten {
    my $written = do1field("Written in");
    return $written;
}
