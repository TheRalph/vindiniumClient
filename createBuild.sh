#!/bin/bash

URL=""
KEY="uviqc5an" # Default is SSITeam
MODE=""
NB_TURNS=""
MAP=""
BROWSER=""

################################################################################
usage()
{
    prog_name=`basename $0`
    echo "Usage: $prog_name [<options>]"
    echo ""
    echo "Options:"
    echo "  -h"
    echo "      Print this help message."
    echo ""
    echo "  -u <url>"
    echo "      The URL of the Vindinium server."
    echo ""
    echo "  -k <key>"
    echo "      Your secret key given on the Vindinium server."
    echo "      Default ID is SSITeam."
    echo ""
    echo "  -a <mode>"
    echo "      The game mode (API) to start."
    echo "      Available modes: training (0) or arena (1)."
    echo "      Default: training"
    echo ""
    echo "  -m <map>"
    echo "      The map to play with."
    echo "      Available maps: m1..m6."
    echo "      Only available for the training mode."
    echo ""
    echo "  -n <nb turns>"
    echo "      The number of turns to play."
    echo "      Only available for the training mode."
    echo ""
    echo "  -b <behaviour>"
    echo "      The behavior module you want to activate for this game."
    echo ""
    echo "  -w <browser>"
    echo "      The browser your want to open to display the game."
    echo ""
}

################################################################################
parse_opt()
{
    while getopts ":hu:k:a:m:n:w:" option
    do
        case $option in
            h) usage ; exit 0 ;;
            u) URL=${OPTARG} ;;
            k) KEY=${OPTARG} ;;
            a) MODE=${OPTARG} ;;
            m) MAP=${OPTARG} ;;
            n) NB_TURNS=${OPTARG} ;;
            w) BROWSER=${OPTARG} ;;
            *) usage ; exit 1 ;;
        esac
    done
}

################################################################################
parse_opt $@
shift $(($OPTIND - 1))

rm -rf build
mkdir -p build/dump
cd build

CMD_LINE="../bin/vindiniumLauncher"
if [ -n "$URL" ] ; then
    CMD_LINE="$CMD_LINE --url=$URL"
fi
if [ -n "$MODE" ] ; then
    CMD_LINE="$CMD_LINE --mode=$MODE"
fi
if [ -n "$MAP" ] ; then
    CMD_LINE="$CMD_LINE --map=$MAP"
fi
if [ -n "$TURNS" ] ; then
    CMD_LINE="$CMD_LINE --turns=$TURNS"
fi
if [ -n "$BROWSER" ] ; then
    CMD_LINE="$CMD_LINE --browser=$BROWSER"
fi

# random
cat > testIA.sh << EOF
#!/bin/sh

$CMD_LINE --behaviour=random -- $KEY

EOF
chmod u+x testIA.sh

# botMitch
cat > mitchIA.sh << EOF
#!/bin/sh

$CMD_LINE --behaviour=botMitch -- $KEY

EOF
chmod u+x mitchIA.sh

# TheRalphBot
cat > TheRalphBot.sh << EOF
#!/bin/sh

$CMD_LINE --behaviour=TheRalphBot -- $KEY

EOF
chmod u+x TheRalphBot.sh

ln -s ../bin/vindiniumLauncher vindiniumLauncher

cmake ..
cd ..

