#The Comp-hist perl module for parsing.
package ParseCH;

use strict;

require Exporter;
use vars        qw($VERSION @ISA @EXPORT %EXPORT_TAGS);

# set version number
$VERSION = 1.6; #CVS version

@ISA = qw(Exporter);
@EXPORT = qw(&docom &doname &doref &dotype &dostatus &doinf &doto &dofrom &doweight);
%EXPORT_TAGS = ();

#non-exported globals (nothing is exported. yet.)
use vars    qw(@node @to @from @weight $file %name %color %shape);

@node = '';
@to = '';
@from = '';
@weight = '';
$file = '';
%name = ();
%color = ();
%shape = ();

#First, an unexported function or two

sub onode {
    my $node = $1 if /^(\S+)/;
    return $node;
}

sub linkup {
    open FILE, $_[0];
    while(<FILE>){
	my $node = onode();
	if(/.*Aka (\S+)/){
	    push(@to, $node);
	    push(@from, $1);
	    push(@weight, "aka");
	}
        if(/.*Successor to (\S+)/){
            push(@to, $node);
            push(@from, $1);
            push(@weight, "successor");
        }
        if(/.*Code taken from (\S+)/){
            push(@to, $node);
            push(@from, $1);
            push(@weight, "code");
        }
        if(/.*Influenced by (\S+)/){
            push(@to, $node);
            push(@from, $1);
            push(@weight, "influence");
        }
        if(/.*Runs on (\S+)/){
            push(@to, $node);
            push(@from, $1);
            push(@weight, "runs");
        }
        if(/.*Ran on (\S+)/){
            push(@to, $node);
            push(@from, $1);
            push(@weight, "ran");
        }
        if(/.*Written in (\S+)/){
            push(@to, $node);
            push(@from, $1);
            push(@weight, "written");
        }
    }
}

#Now the exported ones

sub docom {
    foreach $file (@_){
	while(<$file>){
	    s/#.*//;
	}
    }
}

sub doname {
    foreach $file (@_){
	while(<$file>){
	    my $node = onode;
	    if(/.*Name: (.*)/){ $name{$node} = $1; }
	}
    }
    return %ParseCH::name;
}

sub doref {
    my @ref = ();
    my $item = '';
    my %seen = ();
    my @uref = ();
    foreach $file (@_){
	while(<$file>){
	    push(@ref, $1) if /.*Reference: (.*)/;
	}
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
	my $node = onode();
        if(/.*Type: hardware/i){ $color{$node} = "color: blue"; }
        if(/.*Type: OS/i){ $color{$node} = "color: red"; }
        if(/.*Type: language/i){ $color{$node} = "color: green"; }
        if(/.*Type: standard/i){ $color{$node} = "color: yellow"; }
        if(/.*Type: other/i){ $color{$node} = "color: magenta"; } #You don't know about this line. Go away.
	if(/.*Type: company/i){ $color{$node} = "color: cyan"; }
        if(/.*Type: announcement/i){ $color{$node} = "color: gold"; }
    }
    return %ParseCH::color;
}

sub dostatus {
    open FILE, $_[0];
    while(<FILE>){
	my $node = onode;
	if(/.*Status: released/i){ $shape{$node} = "shape: box"; }
        if(/.*Status: internal/i){ $shape{$node} = "shape: triangle"; }
        if(/.*Status: continual evolution/i){ $shape{$node} = "shape: ellipse"; }
        if(/.*Status: prototype/i){ $shape{$node} = "shape: rhomb"; }
        if(/.*Status: research/i){ $shape{$node} = "shape: triangle"; }
    }
    return %ParseCH::shape;
    }

sub doinf {
    my @inf = ();
    my $item = '';
    my %seen = ();
    my @uinf = ();
    foreach $file (@_){
	while(<$file>){
	    push(@inf, $1) if /.*Info: (.*)/;
	}
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

sub doto {
    foreach $file (@_){
	linkup;
	}
    return @to;
}

sub dofrom {
    foreach $file (@_){
        linkup;
        }
    return @from;
}

sub doweight {
    foreach $file (@_){
        linkup;
        }
    return @weight;
}

END { }
