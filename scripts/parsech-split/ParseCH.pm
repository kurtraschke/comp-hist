package ParseCH;

use strict;

BEGIN{
  use Exporter ();
  use vars qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);
  $VERSION = do { my @r = (q$Revision: 1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r };
  @ISA = qw(Exporter);
  @EXPORT = qw(&parser);
  %EXPORT_TAGS = (ALL => [qw(&parser %Name %Year %Month %Day %Reference %Info %Type %Status %Linkto)]);
  @EXPORT_OK = qw(%Name %Year %Month %Day %Reference %Info %Type %Status %Linkto);
}

use vars qw(%Name %Year %Month %Day %Reference %Info %Type %Status %Linkto);

sub parser {
  no strict "refs";
  my $node;
  foreach my $file (@_){
    open $file, $file;
    while(<>){
      s/\#.*//;
      if(/^(\S+)/){ $node = $1; }
      
      if(/.*Name: (.*)/){ $Name{$node} = $1; }
      
      if (/.*Founded.*(\d\d\d\d)-(\d\d)-(\d\d)/){
	$Year{$node}=$1; 
	$Month{$node}=$2;
	$Day{$node}=$3;
      }
      elsif (/.*Founded.*(\d\d\d\d)-(\d\d)/) {
	$Year{$node}=$1;
	$Month{$node}=$2;
      }
      elsif (/.*Founded.*(\d\d\d\d)/) {
	$Year{$node}=$1;
      }
      
      if (/.*Date:.*(\d\d\d\d)-(\d\d)-(\d\d)/){
	$Year{$node}=$1;
	$Month{$node}=$2;
	$Day{$node}=$3;
      }
      elsif (/.*Date:.*(\d\d\d\d)-(\d\d)/) {
	$Year{$node}=$1;
	$Month{$node}=$2;
      }
      elsif (/.*Date:.*(\d\d\d\d)/) {
	$Year{$node}=$1;
      }
      
      if(/.*Reference: (.*)/){ $Reference{$node} = $1; }
      
      if(/.*Info: (.*)/){ $Info{$node} = $1; }
      
      if(/.*Type: hardware/){ $Type{$node} = "hardware"; }
      elsif(/.*Type: os/i){ $Type{$node} = "os"; }
      elsif(/.*Type: language/){ $Type{$node} = "language"; }
      elsif(/.*Type: standard/){ $Type{$node} = "standard"; }
      elsif(/.*Type: other/){ $Type{$node} = "other"; }
      elsif(/.*Type: company/){ $Type{$node} = "company"; }
      elsif(/.*Type: announcement/){ $Type{$node} = "announcement"; }
      
      if(/^\tStatus: released/i){ $Status{$node} = "released"; }
      elsif(/^\tStatus: internal/i){ $Status{$node} = "internal"; }
      elsif(/^\tStatus: prototype/i){ $Status{$node} = "prototype"; }
      elsif(/^\tStatus: research/i){ $Status{$node} = "research"; }
      elsif(/^\tStatus: continual evolution/i){ $Status{$node} = "contev"; }
      
      if(/.*Aka (\S+)/) { $Linkto{$node}{$1}="aka"; }
      if (/.*Successor to (\S+)/) { $Linkto{$node}{$1}="successor"; }
      if (/.*Code taken from (\S+)/) { $Linkto{$node}{$1}="codetaken"; }
      if (/.*Influenced by (\S+)/) { $Linkto{$node}{$1}="influenced"; }
      if (/.*Runs on (\S+)/) { $Linkto{$node}{$1}="runson"; }
      if (/.*Ran on (\S+)/) { $Linkto{$node}{$1}="runson"; }
      if (/.*Written in (\S+)/) { $Linkto{$node}{$1}="writtenin"; }
    }
  }
}

1;
