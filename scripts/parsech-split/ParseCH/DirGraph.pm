package ParseCH::DirGraph;

use strict;

BEGIN{
  use Exporter ();
  use vars qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);
  $VERSION = do { my @r = (q$Revision: 1.2 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r };
  @ISA = qw(Exporter);
  @EXPORT = qw();
  %EXPORT_TAGS = (ALL => [qw(&dominmaxYear &dopos &setcolor &setshape &setlinkcolor)]);
  @EXPORT_OK = qw(&dominmaxYear &dopos &setcolor &setshape &setlinkcolor);
}

sub round {
    my($number) = shift;
    return int($number + .5 * ($number <=> 0));
}

sub dominmaxYear {
  my $Maxyear=0;
  my $Minyear=800000;
  foreach my $Year (values %{$_[0]}){
    if($Year gt $Maxyear){ $Maxyear = int($Year); }
    if($Year lt $Minyear){ $Minyear = int($Year); }
  }
  return($Minyear, $Maxyear);
}

sub dopos{
  my(%VPos, $Year);
  my $VScale = shift;
  my $year = shift;
  my $month = shift;
  my %Year = %{$year};
  my %Month = %{$month};
  my($Minyear, $Maxyear) = dominmaxYear(\%Year);
  foreach my $node (keys %Year){
    if(!defined $Month{$node}){
      $Year = int($Year{$node}) - $Minyear;
      $VPos{$node} = $Year * 12;
    }
    else{
      $Year = int($Year{$node}) - $Minyear;
      $VPos{$node} = $Year * 12 + $Month{$node};
    }
    $VPos{$node} = round($VPos{$node}/$VScale);
#    print STDERR "VPos for node $Name{$node} is $VPos{$node}\n";
  }
  return %VPos;
}

sub setcolor{
    my($node, %Color, %Type);
    %Type = %{$_[0]};
    foreach $node (keys %Type){
	$Color{$node} = "blue" if $Type{$node} =~ /^hardware/;
	$Color{$node} = "red" if $Type{$node} =~ /^os/i;
	$Color{$node} = "green" if $Type{$node} =~ /^language/;
	$Color{$node} = "yellow" if $Type{$node} =~ /^standard/;
	$Color{$node} = "cyan" if $Type{$node} =~ /^company/;
	$Color{$node} = "gold" if $Type{$node} =~ /^announcement/;
	$Color{$node} = "magenta" if $Type{$node} =~ /^other/;
    }
    return %Color;
}

sub setshape {
    my($node, %Shape, %Status);
    %Status = %{$_[0]};
    foreach $node (keys %Status){
	if ($Status{$node} =~ /^released/){ $Shape{$node} = "box"; }
	elsif ($Status{$node} =~ /^triangle/){ $Shape{$node} = "triangle";}
	elsif ($Status{$node} =~ /^contev/){$Shape{$node} = "ellipse";}
	elsif ($Status{$node} =~ /^prototype/){$Shape{$node} = "rhomb";}
	elsif ($Status{$node} =~ /^research/){$Shape{$node} = "triangle";}
	else {$Shape{$node} = "box";}
    }
    return %Shape;
}

sub setlinkcolor {
  my($linkcolor, $linkweight, %Linkcolor, %Linkto);
  foreach my $node (keys %{$_[0]}){
    foreach my $linkto (keys %{${$_[0]}{$node}}){
      $linkweight=${$_[0]}{$node}{$linkto};
      if($linkweight eq "aka"){$linkcolor="blue";}
      elsif($linkweight eq "successor"){$linkcolor="red";}
      elsif($linkweight eq "codetaken"){$linkcolor="orange";}
      elsif($linkweight eq "influenced"){$linkcolor="pink";}
      elsif($linkweight eq "runson"){$linkcolor="yellow";}
      elsif($linkweight eq "writtenin"){$linkcolor="green";}
      $Linkcolor{$node}{$linkto}=$linkcolor;
    }
  }
  return %Linkcolor;
}

1;
