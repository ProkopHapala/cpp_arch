#! /bin/gawk -f
{}{
OFMT = "%.16f";
CONVFMT ="%.16f";

if (NR==1 )
	{
        print "# comment"
	}

else if ($1==6)
	{
	$1=" 4.0 "
        print($1);
	}
else if ($1==7)
	{
	$1=" 5.0 "
        print($1);
	}
else if ($1==8)
	{
	$1=" 6.0 "
        print($1);
	}
else if ($1==14)
	{
	$1=" 4.0 "
        print($1);
	}
else if ($1==15)
	{
	$1=" 5.0 "
        print($1);
	}
else if ($1==16)
	{
	$1=" 6.0 "
        print($1);
	}
else if ($1==32)
	{
	$1=" 4.0 "
        print($1);
	}
else if ($1==50)
	{
	$1=" 4.0 "
        print($1);
	}
else if ($1==1)
	{
	$1=" 1.0 "
        print($1);
	}
else if ($1==79)
	{
	$1=" 11.0 "
        print($1);
	}
else
	{
	}
	

}
