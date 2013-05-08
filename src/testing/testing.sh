#!/bin/bash

if [ $# = 7 ]
then 
        REF=$1
        SUB=$2
        OUT=$3
        METHOD=$4
        TOL=$5
        N=$6
        FILE=$7
        
        if [ $METHOD != 1 ] && [ $METHOD != 2 ] && [ $METHOD != 3 ]
        then    echo "Method must be 1, 2 or 3!"
        else
                echo "Test - processing times" > $FILE
                for ((I=0; I<N; I++))
                do      ./geoc_testing $REF $SUB $OUT $METHOD $TOL >> $FILE
                done
        
                echo "Processing times were saved to file $FILE"
        fi
else
        echo "Usage: input_ref input_sub output method tolerance it_number output_file"      
fi


