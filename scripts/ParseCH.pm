#The Comp-hist perl module for parsing. See the end for POD
package ParseCH;

use strict;

require Exporter;
use vars        qw($VERSION @ISA @EXPORT %EXPORT_TAGS);

# set version number
$VERSION = 0.01; #May as well

@ISA = qw(Exporter);
@EXPORT = qw(&docomment &donode &doname &doref &dotype);
%EXPORT_TAGS = ();

#non-exported globals (nothing is exported. yet.)
use vars    qw(@node);

@node = '';

#First, an invisible function

my $onode = sub {
    cout;
    my $node = $1 if /^(\S+)/;
    return $node;
}

#Now the ones they can see

sub docomment {
    s/#.*//;
}

sub donode {
    open FILE, $_[0];
    while(<FILE>){
	$node[$ncount] = $1 if /^(\S+)/;
	$ncount++;
    }
    return @node;
}

sub doname {
    open FILE, $_[0];
    while(<FILE>){
	my $node = &$onode;
	$name{$node} = $1 if /.*Name: (.*)/;
    }
    return %name;
}

sub doref {
    open FILE, $_[0];
    my @ref = ();
    my $item = '';
    my %seen = ();
    my @uref = ();
    while(<FILE>){
	my $node = &$onode;
	$ref[$rcnt] = $1 if /.*Reference: (.*)/;
	$rcnt++;
    }
    #Now, get rid of the duplicates
    foreach $item (@ref){
	unless ($seen{$item}){
	    $seen{$item} = 1;
	    push(@uref, $item);
	}
    }
    return @uref;
}

sub dotype {
    open FILE, $_[0];
    while(<FILE>){
        #This is the whole reason for this module
	my $node = &$onode;
        if(/.*Type: hardware/i){ $color{$node} = "color: blue"; }
        if(/.*Type: OS/i){ $color{$node} = "color: red"; }
        if(/.*Type: language/i){ $color{$node} = "color: green"; }
        if(/.*Type: standard/i){ $color{$node} = "color: yellow"; }
        if(/.*Type: other/i){ $color{$node} = "color: magenta"; } #You don't know about this line. Go away.
	if(/.*Type: company/i){ $color{$node} = "color: cyan"; }
        if(/.*Type: announcement/i){$color{$node} = "color: gold"; }
    }
    return %color;
}
