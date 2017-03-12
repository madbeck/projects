'''
A program that sifts through a CSV dataset on homelessness in the US and answers various questions about it
'''

import csv

def find_number_of_homeless():
    
#the first function finds the number of homeless people given a specific state and year input that I give it
    
    with open("homelessness_data.csv", "rU") as csvfile:
        N = csv.reader(csvfile)
        N.next()
        state = raw_input("What state are you looking for? ")
        year = raw_input("What year do you want? ")

        for row in N: #row represents the columns in the excel
            if row[1] == state and year == "2012":
                print row[3]
            elif row[1] == state and year == "2013":
                print row[2]

def sort_dataset_by_amountinyear():
#sorts the dataset by the year I give it - aka prints the states and their corresponding data in order of value
#printing from least number of homeless to greatest
    
    with open("homelessness_data.csv", "rU") as csvfile:
        N = csv.reader(csvfile)
        N.next()
        year = raw_input("What year are you looking for? ")
        mydict2012 = {} #the first dictionary is made for 2012, so it creates a key and value list of all the states and numbers of 2012
        mydict2013 = {} #the second does the same for 2013
    
        for row in N:
            if year == "2012": 
                mydict2012[row[1]] = int(row[3])
            if year == "2013":
                mydict2013[row[1]] = int(row[2]) #a dictionary of all the state-value pairs in 2013
        #here, it sorts the lists from least to greatest, using 'key = lambda' which is just an anonymous function that allows you to write whatever commands afterwards
        #x[1] is the command that works with the lambda function and tells the function to sort the dictionary by the second value in the dictionary, which in this case is the number of homeless in the state
        print sorted(mydict2012.items(), key = lambda x: (x[1]))
        print sorted(mydict2013.items(), key = lambda x: (x[1]))

sort_dataset_by_amountinyear()

def largest_homeless_count(): #finds the state with the highest number of homeless in 2012 and 2013, essentially the WORST state, and prints the state and whatever the sum is to the shell
    
    with open("homelessness_data.csv", "rU") as csvfile:
        N = csv.reader(csvfile)
        N.next()
        
        largest_number = 0 #this variable will eventually hold the sum of the 2012 and 2013 homeless values of the "worst" state
        largest_state = "" #this will eventually hold the name of the "worst" state
        
        for row in N:
            if int(row[3]) + int(row[2]) > largest_number: #looks at the added 2012 and 2013 numbers for one state at a time
                largest_number = int(row[3]) + int(row[2]) #reassigns the largest_number variable each time the new sum (of the 2012 and 2013 values) is greater than the last and will finish when it has gone through all the values
                largest_state = row[1] #when the above part finishes and finds the largest_number, this will assign the largest_state variable to be the corresponding state in the second column
    
        print "State with the most homeless people overall :", largest_state, ",", largest_number
        #prints the state with largest combined 2012 and 2013 homeless numbers and the sum
        
largest_homeless_count()

