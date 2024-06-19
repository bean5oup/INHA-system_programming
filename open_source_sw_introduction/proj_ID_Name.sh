#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "usage: $0 file1 file2 file3"
    exit 1
else
    for file in "$1" "$2" "$3"; do
        if [ ! -f "$file" ]; then
            echo "usage: $0 file1 file2 file3"
            exit 2
        elif [[ "$file" =~ "teams" ]]; then
            teams="$file"
        elif [[ "$file" =~ "players" ]]; then
            players="$file"
        elif [[ "$file" =~ "matches" ]]; then
            matches="$file"
        fi
    done
fi

echo "************OSS1 - Project1************
*        StudentID :                  *
*        Name :                       *
***************************************"

function printMenu() {
    echo -e "\n[MENU]"
    echo "1. Get the data of Heung-Min Son's Current Club, Appearances, Goals, Assists in players.csv"
    echo "2. Get the team data to enter a league position in teams.csv"
    echo "3. Get the Top-3 Attendance matches in mateches.csv"
    echo "4. Get the team's league position and team's top scorer in teams.csv & players.csv"
    echo "5. Get the modified format of date_GMT in matches.csv"
    echo "6. Get the data of the winning team by the largest difference on home stadium in teams.csv & matches.csv"
    echo "7. Exit"
}

function playerInfo() {
    read -p "Do you want to get the $1's data? (y/n) : " response
    if [[ "$response" != [Yy]* ]]; then
        return 0
    fi
    cat $players | awk -F, 'NR>1&&$1=="'"$1"'"{print "Team:"$4", Apperance:"$6", Goal:"$7", Assist:"$8}'
}

function teamInfo() {
    read -p "What do you want to get the team data of league_position[1~20] : " pos
    cat $teams | awk -F, 'NR>1&&$6=="'"$pos"'"{print $6" "$1" "$2/($2+$3+$4)}'
}

function getMatchesByAttendance() {
    read -p "Do you want to know Top-3 attendance data and average attendance? (y/n) : " response
    if [[ "$response" != [Yy]* ]]; then
        return 0
    fi
    top=$1
    echo -e "***Top-3 Attendance Match***\n"
    tail -n +2 $matches \
    | sort -t ',' -k2nr \
    | head -"$top" \
    | awk -F, '{print $3" vs "$4" ("$1")\n"$2" "$7"\n"}'
}

function getTeamsPos8Score() {
    read -p "Do you want to get each team's ranking and the highest-scoring player? (y/n) : " response
    if [[ "$response" != [Yy]* ]]; then
        return 0
    fi
    
    IFS=$'\n'
    for team in $(tail -n +2 $teams | sort -t ',' -k6n); do 
        read pos8name <<< `echo $team | awk -F, '{print $6" "$1}'`
        echo $pos8name
        pos="${pos8name%% *}"
        team_name="${pos8name#* }"
        max_score=""
        for player in $(tail -n +2 $players | grep -a "$team_name" | sort -t ',' -k7nr); do
            score8name=`echo $player | awk -F, '{print $7" "$1}'`
            score="${score8name%% *}"; player_name="${score8name#* }"
            if [ -z "$max_score" ]; then
                max_score="$score"
            fi
            if [ "$score" = "$max_score" ]; then
                echo $player_name $score 
            fi
        done
        echo 
    done
}

function getDateMatches() {
    read -p "Do you want to modify the format of date? (y/n) : " response
    if [[ "$response" != [Yy]* ]]; then
        return 0
    fi
    tail -n +2 $matches \
    | sed -E '/^Jan /{s/^Jan/01/; b}; /^Feb /{s/^Feb/02/; b}; /^Mar /{s/^Mar/03/; b}; /^Apr /{s/^Apr/04/; b}; /^May /{s/^May/05/; b}; /^Jun /{s/^Jun/06/; b}; /^Jul /{s/^Jul/07/; b}; /^Aug /{s/^Aug/08/; b}; /^Sep /{s/^Sep/09/; b}; /^Oct /{s/^Oct/10/; b}; /^Nov /{s/^Nov/11/; b}; /^Dec /{s/^Dec/12/; b}' \
    | sed -E 's/(0[1-9]|1[0-2]) (0[0-9]|[12][0-9]|3[01]) (20[0-9]{2}) - ((2[0-4]|1[0-9]|[1-9]):[0-5][0-9](am|pm))/\3\/\1\/\2 \4/' \
    | cut -d, -f 1 \
    | sed -n '1,10p'
}

function getDiff() {
    list=()
    IFS=$'\n'
    for team in $(tail -n +2 $teams); do
        list+=("`echo $team | awk -F, '{print $1}'`")
    done
    COLUMNS=80
    PS3="Enter your team number : "
    select team in "${list[@]}"; do
        if [ -n "$team" ]; then
            IFS=$'\n'
            max_score=""
            for result in `tail -n +2 $matches \
            | awk -F, '$3=="'"$team"'"{print $1","$3","$4","$5","$6","$5-$6}' \
            | sort -t ',' -k6nr`; do
                score=`echo $result | awk -F, '{print $6}'`
                if [ -z "$max_score" ]; then
                    max_score="$score"
                fi
                if [ "$score" = "$max_score" ]; then
                    echo $result | awk -F, '{print "\n"$1"\n"$2" "$4" vs "$5" "$3}'
                fi
            done
            break
        else
            echo "Invalid selection. Please try again."
        fi
    done
}

while true; do
    printMenu
    read -p "Enter your CHOICE (1~7) : " choice
    case $choice in
        1)
            playerInfo "Heung-Min Son"
            ;;
        2)
            teamInfo
            ;;
        3)
            getMatchesByAttendance 3
            ;;
        4)
            getTeamsPos8Score
            ;;
        5)
            getDateMatches
            ;;
        6)
            getDiff
            ;;
        7)
            echo -e "Bye!\n"
            break
            ;;
        *)
            ;;
    esac
done
