'''
A generate and filter program that finds all the legal colorings of a graph where no two connected nodes can have the same color.
Utilizes function is_a_legal_coloring(), included farther down, which checks to see if a certain graph coloring is legal.

ex:
>>> print collect_legal_colorings("ABC", "rgb", "AB BC") # arbitrary states
Ar Bg Cr
Ar Bg Cb
Ar Bb Cr
Ar Bb Cg
Ag Br Cg
Ag Br Cb
Ag Bb Cr
Ag Bb Cg
Ab Br Cg
Ab Br Cb
Ab Bg Cr
Ab Bg Cb
<BLANKLINE>


def return_smaller1(states, combos_we_want, colors, borders): #reduces collect_all_colorings to a smaller problem size

    if len(states) == 0: #this shouldn't happen often but returns an empty string if there are no states
        return ""
    elif len(states) == 1: #base case when there is only one state
        if combos_we_want == "": #I have two cases here: 1) for when the empty string is still empty (which will happen only when the question has one state) and 2) for when combos_we_want has possible colorings from previous smaller instances
            return (states + colors[0] + "\n" + 
                    states + colors[1] + "\n" +
                    states + colors[2] + "\n")
        else: #if combos_we_want isn't a blank string (because it got redefined below, we will want to include it in our return statement because it has important smaller answers to our problem
            #3 pairs of if statements to look at three different base cases
            #the function combines the filter with the generator to check if each base case is True before putting it in the final return statement
            if my_is_a_legal_coloring(combos_we_want + " " + states + colors[0], borders) == False:
                answer1 = ""
            if my_is_a_legal_coloring(combos_we_want + " " + states + colors[0], borders) == True:
                answer1 = combos_we_want + " " + states + colors[0] + "\n"
            if my_is_a_legal_coloring(combos_we_want + " " + states + colors[1], borders) == False:
                answer2 = ""
            if my_is_a_legal_coloring(combos_we_want + " " + states + colors[1], borders) == True:
                answer2 = combos_we_want + " " + states + colors[1] + "\n"
            if my_is_a_legal_coloring(combos_we_want + " " + states + colors[2], borders) == False:
                answer3 = ""
            if my_is_a_legal_coloring(combos_we_want + " " + states + colors[2], borders) == True:
                answer3 = combos_we_want + " " + states + colors[2] + "\n"
            return (answer1 +
                    answer2 +
                    answer3)

    else:
        if combos_we_want == "": #below commands run when len(states) is greater than one and use recursion to shrink the number of states until we reach our base case
            #I used two cases again for when combos_we_want is still an empty string (before it gets redefined) and when we have already redefined it
            #like above, if we don't do two cases there will be an extra space in the return statement that includes combos_we_want that is still an empty string
            return (return_smaller1(states[1:], states[0] + colors[0], colors, borders) +
                    return_smaller1(states[1:], states[0] + colors[1], colors, borders) + 
                    return_smaller1(states[1:], states[0] + colors[2], colors, borders))
        else:
            return (return_smaller1(states[1:], combos_we_want + " " + states[0] + colors[0], colors, borders) +
                    return_smaller1(states[1:], combos_we_want + " " + states[0] + colors[1], colors, borders) + 
                    return_smaller1(states[1:], combos_we_want + " " + states[0] + colors[2], colors, borders))

def collect_legal_colorings(states, colors, borders):
    assert colors == "rgb" or "rbg" or "gbr" or "grb" or "bgr" or "brg" #this makes sure our colors are red, blue, and green, no more, no less
    combos_we_want = "" #define combos_we_want as an empty string that will soon be redefined as we create smaller instances of the problem
    return return_smaller1(states, combos_we_want, colors, borders) #ADDED BORDERS


############################################################################
#CODE FOR COLLECT ALL COLORINGS - this is the generator
def return_smaller(states, combos_we_want, colors):

    if len(states) == 0: #this shouldn't happen often but returns an empty string if there are no states
        return ""
    
    elif len(states) == 1: #base case when there is only one state
        if combos_we_want == "": #I have two cases here: 1) for when the empty string is still empty (which will happen only when the question has one state) and 2) for when combos_we_want has possible colorings from previous smaller instances
            return (states + colors[0] + "\n" + 
                    states + colors[1] + "\n" +
                    states + colors[2])
        else: #if combos_we_want isn't a blank string (because it got redefined below, we will want to include it in our return statement because it has important smaller answers to our problem
            return (combos_we_want + " " + states + colors[0] + "\n" + 
                    combos_we_want + " " + states + colors[1] + "\n" + 
                    combos_we_want + " " + states + colors[2])
    else:
        if combos_we_want == "": #below commands run when len(states) is greater than one and use recursion to shrink the number of states until we reach our base case
            #I used two cases again for when combos_we_want is still an empty string (before it gets redefined) and when we have already redefined it
            #like above, if we don't do two cases there will be an extra space in the return statement that includes combos_we_want that is still an empty string
            return (return_smaller(states[1:], states[0] + colors[0], colors) + "\n" +
                    return_smaller(states[1:], states[0] + colors[1], colors) + "\n" + 
                    return_smaller(states[1:], states[0] + colors[2], colors))
        else:
            return (return_smaller(states[1:], combos_we_want + " " + states[0] + colors[0], colors) + "\n" +
                    return_smaller(states[1:], combos_we_want + " " + states[0] + colors[1], colors) + "\n" + 
                    return_smaller(states[1:], combos_we_want + " " + states[0] + colors[2], colors))

def collect_all_colorings(states, colors):
    assert colors == "rgb" or "rbg" or "gbr" or "grb" or "bgr" or "brg" #this makes sure our colors are red, blue, and green, no more, no less
    combos_we_want = "" #define combos_we_want as an empty string that will soon be redefined as we create smaller instances of the problem
    return return_smaller(states, combos_we_want, colors)

    # The next five lines ensure that, when doing graphics runs, we print out the parameters
    we_are_doing_coloring_enumeration()
    want_to_print_stuff = not are_we_doing_doctest()
    if want_to_print_stuff:
        print "Calling collect_legal_colorings"
        print ">>> collect_legal_colorings('" + states + "', '" + colors + "', '" + borders + "')"

    MODE='mine'  # set to 'test samples', 'answer key', or 'mine'
    
    if MODE=='mine':
        answer = True
        
        # REPLACE THIS WITH YOUR ALGORITHM for collect_legal.  Set the variable answer instead of doing a return if you want to see it printed automatically
    elif MODE=='test samples' or MODE=='answer key':
        try:
            """ Call all sample answers, see what answers we get ... """
            from sample_answers.cs105.graph_coloring.enumeration_samples import collect_legal_colorings_samples_all
            from sample_answers.cs105.graph_coloring.enumeration_samples import collect_legal_colorings_samples_correct
            # set the following to only use the correct one!
            enumeration_samples_just_do_the_right_ones = (MODE=='answer key')
        
            if enumeration_samples_just_do_the_right_ones:
                answer = collect_legal_colorings_samples_correct(states, colors, borders)
            else:
                answer = collect_legal_colorings_samples_all(states, colors, borders)
        except:
            print "Hmmmm... can't find sample answers. This shouldn't happen on the CS teaching lab computers"
            print " If you are running this program on another computer, you'll have to wait to check"
            print " your test suite against the sample answers when you're back in the lab."
            print " (Remember to Team->Commit on your computer and Team->Update in the lab.)"
            answer = ""
    else:
        answer = 'ERROR: You need to set MODE correctly in graph_coloring.py'

    # Leave the next line alone, too, to ensure proper printing in the GUI
    we_are_doing_coloring_enumeration(False) # done with this enumeration

    if want_to_print_stuff:
        print answer
    return answer


# The next two functions are used to make the test suite less dependent on ordering.
# The first uses some fancy Python stuff from "from string import *";
#    the second basic recursive design and functions-in-functions
# CS105 students should NOT worry about understanding _how_ they work!

from string import *

# how many solutions are in a set of colorings (just counts the number of \n's)
def count_solutions(set_of_colorings):
    precondition(True)  # Always does *something*, but complains in a useful way about bad parameters
    
    if set_of_colorings == None:
        return ("Error: count_solutions called with None as a parameter.\n"+
                "       This typically means the function that produced its parameter lacks a return in some case")
    elif not isinstance(set_of_colorings, basestring):
        return ("Error: count_solutions called with a non-string parameter.\n"+
                "       This typically means the function that produced its parameter returned something else")

    else:
        return set_of_colorings.count("\n")

# true or false --- does set_of_colorings have the coloring?
def has_this_coloring(set_of_colorings, this_coloring):
    precondition(True)  # Always does *something*, but complains in a useful way about bad parameters
    
    if set_of_colorings == None:
        return ("Error: count_solutions called with None as a set_of_colorings parameter.\n"+
                "       This typically means the function that produced its first parameter lacks a return in some case")
    elif not isinstance(set_of_colorings, basestring):
        return ("Error: count_solutions called with a non-string as a set_of_colorings parameter.\n"+
                "       This typically means the function that produced its first parameter returned something else")
    elif not isinstance(this_coloring, basestring):
        return ("Error: count_solutions called with a non-string as a this_coloring parameter.\n"+
                "       Make sure the second parameter is a string")
    elif "\n" in this_coloring:
        return ("Error: count_solutions called with a multi-line string as a this_coloring parameter.\n"+
                "       Make sure the second parameter just a single coloring")

    else:

        def list_has_coloring(list_of_colorings, this_one):
            def same_coloring(c1, c2):
                def subset_coloring(c1, c2):  # are all state/color pairs in c1 also in c2?
                    if (len(c1) < 2):
                        return True
                    else:
                        return c1[0:2] in c2 and subset_coloring(c1[3:], c2)
                return subset_coloring(c1, c2) and subset_coloring(c2, c1)
            if len(list_of_colorings) == 0:
                return False
            else:
                return (same_coloring(list_of_colorings[0], this_one) or
                        list_has_coloring(list_of_colorings[1:], this_one))
        return list_has_coloring(set_of_colorings.split("\n"), this_coloring)


'''
Below code is FILTER portion of generate and filter program and determine whether or not a proposed coloring for a graph (ex: US state map) is legal.
The function takes a list of states and their colors and a list of the states that each borders.

is_a_legal_coloring should return True if the coloring is legal, and False otherwise
'''

def my_is_a_legal_coloring(possible_coloring, borders): #testing if a given coloring is good or not - filter method of generate and filter
    return my_is_a_legal_coloring1(possible_coloring, possible_coloring, borders)
    
def my_is_a_legal_coloring1(possible_coloring, og_colorings, borders): #a second function that adds the parameter og_colorings without adding it to all my test cases
    #og_colorings retains the original proposed coloring and will not be altered by the recursion process -- only possible_coloring will be altered
    #the reason for this is that I still want my precondition in find_the_color_of to work (the precondition that makes sure whatever node we're considering is in possible_coloring), and it won't be effective if possible_coloring keeps getting altered
    #my_is_a_legal_coloring() is still what is being called in the test suite, but my_is_legal_coloring1() contains the actual algorithm of the function
    #uses recursion to create smaller cases of our original problem
    assert len(possible_coloring) > 0 # there must be at least one node with a color
    assert len(borders) >= 0 #there must be at least 0 connected nodes
    
    if len(possible_coloring) == 2:
        return True
    else:
        smaller_case = possible_coloring[3:]
        if my_is_a_legal_coloring1(smaller_case, og_colorings, borders) == True: 
            return is_last_node_legal(possible_coloring[:2], smaller_case, borders)
        else:
            return False

def is_last_node_legal(last_node, possible_coloring, borders):
   
   if len(borders) == 0:
        return True
    elif last_node[0] == borders[0] or last_node[0] == borders[1]:
        if last_node[0] == borders[1]:
            if last_node[1] == find_the_color_of(borders[0], possible_coloring, True):
                return False
            else:
                return is_last_node_legal(last_node, possible_coloring, borders[3:])
                #return is_last_node_legal(last_node, og_colorings, borders[3:])
        else: #else statement will run when last_node[0] == borders[0], aka if the first if statement is not satisfied
            if last_node[1] == find_the_color_of(borders[1], possible_coloring, True):    
                return False
            else:
                #return is_last_node_legal(last_node, og_colorings, borders[3:])
                return is_last_node_legal(last_node, possible_coloring, borders[3:])
        
            
    elif last_node[0] not in borders: #will return True if there is a node that is not connected to any others because it won't matter what color this node is
        return True
    
    else:
        return is_last_node_legal(last_node, possible_coloring, borders[3:])
    
def find_the_color_of(node, possible_coloring, first_time): #uses recursion to check if node(s) attached to the last node have the same color
    if node not in possible_coloring:
        return "z" #this will never show up in a real coloring example and thus will never equal r, g, or b
        
    elif node == possible_coloring[0]:
        colors = 'rgb'
        assert possible_coloring[1] in colors # if it's not, we'll know
        return possible_coloring[1]
    else:
        return find_the_color_of(node, possible_coloring[3:], False)
  
