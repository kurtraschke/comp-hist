package ParseCH::ExpNick;

BEGIN{
  use Exporter ();
  use vars qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);
  $VERSION = do { my @r = (q$Revision: 1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r };
  @ISA = qw(Exporter);
  @EXPORT = qw(&expnick);
  %EXPORT_TAGS = ();
  @EXPORT_OK = qw();
}

sub expnick {
  return "Aka" if $_[0] =~ /aka/;
  return "Successor to" if $_[0] =~ /successor/;
  return "Code taken from" if $_[0] =~ /codetaken/;
  return "Influenced by" if $_[0] =~ /influenced/;
  return "Runs on" if $_[0] =~ /runson/;
  return "Written in" if $_[0] =~ /writtenin/;
}

1;
