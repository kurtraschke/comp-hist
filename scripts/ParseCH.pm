#The Comp-hist perl module for parsing. See the end for POD
package ParseCH;

use strict;

require Exporter;
use vars        qw($VERSION @ISA @EXPORT %EXPORT_TAGS);

# set version number
$VERSION = 0.01; #May as well

@ISA = qw(Exporter);
@EXPORT = qw(&docomment &donode &doname &doref &dotype &dostatus &doinf);
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
	my $node[$ncount] = $1 if /^(\S+)/;
	my $ncount++;
    }
    return @node;
}

sub doname {
    open FILE, $_[0];
    while(<FILE>){
	my $node = &$onode;
	my $name{$node} = $1 if /.*Name: (.*)/;
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
	my $rcnt++;
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
        if(/.*Type: hardware/i){ my $color{$node} = "color: blue"; }
        if(/.*Type: OS/i){ my $color{$node} = "color: red"; }
        if(/.*Type: language/i){ my $color{$node} = "color: green"; }
        if(/.*Type: standard/i){ my $color{$node} = "color: yellow"; }
        if(/.*Type: other/i){ my $color{$node} = "color: magenta"; } #You don't know about this line. Go away.
	if(/.*Type: company/i){ my $color{$node} = "color: cyan"; }
        if(/.*Type: announcement/i){ my $color{$node} = "color: gold"; }
    }
    return %color;
}

sub dostatus {
    open FILE, $_[0];
    while(<FILE>){
	my $node = &$onode;
	if(/.*Status: released/i){ my $shape{$node} = "shape: box"; }
        if(/.*Status: internal/i){ my $shape{$node} = "shape: triangle"; }
        if(/.*Status: continual evolution/i){ my $shape{$node} = "shape: ellipse"; }
        if(/.*Status: prototype/i){ my $shape{$node} = "shape: rhomb"; }
        if(/.*Status: research/i){ my $shape{$node} = "shape: triangle"; }
    }
    return %shape
    }

sub doinf {
    open FILE, $_[0];
    my @inf = ();
    my $item = '';
    my %seen = ();
    my @uinf = ();
    while(<FILE>){
        my $node = &$onode;
        $inf[$icnt] = $1 if /.*Info: (.*)/;
        my $icnt++;
    }
    #Now, get rid of the duplicates
    foreach $item (@inf){
        unless ($seen{$item}){
            $seen{$item} = 1;
            push(@uinf, $item);
        }
    }
    return @uinf;
}

