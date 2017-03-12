'''
A program that sifts through
'''

import csv

def find_number_of_homeless():
    
#the first function finds the number of homeless people given a specific state and year input that I give it
    
    with open("homelessness_data.csv", "rU") as csvfile:
        N = csv.reader(csvfile)
        N.next()
        state = raw_input("What state are you looking for? ") #Because the lab instructions ask for a state and year INPUT, the best way to answer this question seems to be by having a sort of interactive question in the shell that gives you information based on the information you type into it
        #Dylan showed me this! 'raw_input' is a built in python functin that prints anything into the shell and uses your answer to dictate how the program runs
        #Here, I've set a variable called 'state' that will eventually take whatever I type into the shell and look for it in the dataset
        year = raw_input("What year do you want? ")
        #this part asks for a year, so that the program will find the exact number of homeless people in the specified state and year

        for row in N: #row represents the columns in the excel
            if row[1] == state and year == "2012":
                print row[3]
            #this means that if the program finds the state WE type in the shell in the second column of the excel spreadsheet AND the year WE give it is 2012, it will print the corresponding number of homeless in that state in 2012 (the 4th column or row[3])
            #the 'and' here requires that both statements be true for the print statement to work
            elif row[1] == state and year == "2013":
                print row[2]
            #this column does the same thing, but for when the case that we type in 2013 instead of 2012
            #I use an elif statement here because it tells the function ONLY to perform the commands beneat it if the first if statement is not satisfied; if it is satisfied, the program will simply end there
            #if I used another if statement, the program would still work, but it would look over both statements regardless of whether the first one applied or not
            #the elif statement is slightly more correct because if I typed in 2012, there would be no need for the function to run the second command and see if I typed in 2013
           
find_number_of_homeless()
#finally, I call the function to tell the program to run it
#the questions "What state...?" and "What year..." will be printed into the shell one at a time, and only after the person types in the answers to both questions will it run
#if I typed in a state that didn't exist or if I type in a year other than 2012 or 2013, no answer will print, but no error message shows up either

def sort_dataset_by_amountinyear():

#this function "sorts" the dataset by the year I give it
#"sorts" means that it will print the States and their corresponding data in order of value
#I chose to print them in order of least number of homeless to greatest
    
    with open("homelessness_data.csv", "rU") as csvfile:
        N = csv.reader(csvfile)
        N.next()
        year = raw_input("What year are you looking for? ")
        #again, I use the raw-input function to prompt the person to type in which year they are looking for        


        #this below is the key part of the function and uses dictionaries to create a list of keys (the states) and values (the number of homeless people)
        #with the dictionaries, I'll be able to sort the lists by VALUE (the number of homeless people in each state) and the dictionary will reorder itself but keeping the values paired with their corresponding states
        mydict2012 = {} #the first dictionary is made for 2012, so it creates a key and value list of all the states and numbers of 2012
        mydict2013 = {} #the second does the same for 2013
        #essentially, these operate like variables that I will be redefined in the loop below
    
        for row in N:
            if year == "2012": 
                mydict2012[row[1]] = int(row[3])
            #this function sets up a dictionary with the state mapped to the number of homeless in each state
            #again, this means that it will be a lists of pairs - states and their values - that are appended to dictionary in the order they appear in the dataset

            if year == "2013":
                mydict2013[row[1]] = int(row[2]) #a dictionary of all the state-value pairs in 2013


        #these print statements are where the sorting of the lists actually gets completed
        #I got the information for how to sort these by value then key from this website: http://stackoverflow.com/questions/7742752/sorting-a-dictionary-by-value-then-by-key
        #here, it sorts the lists from least to greatest, using 'key = lambda' which is just an anonymous function that allows you to write whatever commands afterwards
        #the x[1] part is the command that works with the lambda functino and tells the function to sort the dictionary by the second value in the dictionary, which in this case is the number of homeless in the state
        print sorted(mydict2012.items(), key = lambda x: (x[1]))
        print sorted(mydict2013.items(), key = lambda x: (x[1]))

sort_dataset_by_amountinyear() #calls the function

def sort_the_lists(any_list): #this function was my attempt to sort any list manually BEFORE I learned that we could use the built in sort function in python
#I don't want to delete it, but PLEASE IGNORE...
    
        for roundnumber in range(len(any_list)-1,0,-1):3
        for i in range(roundnumber):
            if any_list[i] > any_list[i+1]:
                temporary = any_list[i+1]
                any_list[i+1] = any_list[i]
                any_list[i] = temporary
        return any_list
            
#sort_the_lists(myList2012)
#sort_the_lists(myList2013)
#sort_dataset_by_amountinyear()


def largest_homeless_count(): #the final function finds the state with the highest number of homeless in 2012 and 2013, essentially the WORST state, and prints the state and whatever the sum is to the shell
    
    with open("homelessness_data.csv", "rU") as csvfile:
        N = csv.reader(csvfile)
        N.next()
        
        largest_number = 0 #this variable will eventually hold the sum of the 2012 and 2013 homeless values of the "worst" state
        largest_state = "" #this will eventually hold the name of the "worst" state
        
        
        for row in N:
            if int(row[3]) + int(row[2]) > largest_number: #this loop is cool because it makes sure that the function is looking at the added 2012 and 2013 numbers for one state at a time
                #it goes through the function the same way, but is looking at the sum of two columns in the same row as it does this
                largest_number = int(row[3]) + int(row[2]) #the loop will reassign the largest_number variable each time the new sum (of the 2012 and 2013 values) is greater than the last and will finish when it has gone through all the values
                #you need to use int() because otherwise the program will just input the numbers are strings and won't be able to add them numerically
                largest_state = row[1] #when the above part finishes and finds the largest_number (the largest sum of homeless values), this part assigns the largest_state variable to be the corresponding state in the second column
    
        print "State with the most homeless people overall :", largest_state, ",", largest_number
        #prints the state with largest combined 2012 and 2013 homeless numbers and the sum
        
largest_homeless_count()

