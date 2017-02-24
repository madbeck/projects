"""
The below program creates a binary tree object and utilizes that object in an animal-themed 20-questions game.
The original game starts with 20 questions but gets "smarter" as you play, adding questions when it guesses the user's animal incorrectly.
"""

class Binary_Tree:

    #constructors!
    def __init__(self, val, LC, RC):
        precondition((isinstance(LC, Binary_Tree) or LC is None or isinstance(LC, int) or isinstance(LC, str)) and
                     (isinstance(RC, Binary_Tree) or RC is None or isinstance(RC, int) or isinstance(RC, str)))
        self.value = val
        if isinstance(LC, int) and isinstance(RC, int):
             self.leftchild = Binary_Tree(LC, None, None) #left child is the left node of the original
             self.rightchild = Binary_Tree(RC, None, None) #right child is the right node
        elif isinstance(LC, str) and isinstance(RC, str):
            self.leftchild = Binary_Tree(LC, None, None)
            self.rightchild = Binary_Tree(RC, None, None)
        else:
            self.leftchild = LC
            self.rightchild = RC
            
    #3 mutators below
    def change_root_value(self, new_value):
        self.value = new_value
    #Binary_Tree(x, y, z).change_root(a) changes x into a, only modifying the root value
    #Binary_Tree(a, y, z) builds the same tree

    def add_right_child(self, new_node): #from online textbook 6.5
        self.rightchild = Binary_Tree(new_node, None, None)
    #Binary_Tree(x, y, z).add_right_child(a) changes z into a, modifying the right child
    ##Binary_Tree(x, y, a) buils the same tree
           
    def add_left_child(self, new_node):
        self.leftchild = Binary_Tree(new_node, None, None)
    #Binary_Tree(x, y, z).add_right_child(a) changes y into a, modifying the left child
    #Binary_Tree(x, a, z) builds the same tree
           
    #accessors    
    def get_root_val(self):
        return self.value
    #Binary_Tree(x, y, z).get_root_value() = x
    #x will either be an integer or string
    
    def get_right_child(self):
        return self.rightchild
    #Binary_Tree(x, y, z).get_right_child() = z
    #z can be an integer, string, binary tree, or None
    
    def get_left_child(self):
        return self.leftchild
    #Binary_Tree(x, y, z).get_left_child() = y
    #y can also be an integer, string, binary tree, or None
    
    def is_leaf(self):
        if isinstance(self.rightchild, type(None)) and isinstance(self.leftchild, type(None)):
            return True
        else:
            return False
    #Binary_Tree(x, y, z).is_leaf()
    #if y and z are both None, then it is a leaf, and the method will return True
    #in every other case, the answer is false because the node has some children beneath it other than none

    def __eq__(self, tree2): #determines whether or not two trees are equal
        if self.get_root_val() != tree2.get_root_val(): #base case - if this is false, the whole thing is false
            return False
        else:
            if isinstance(self.get_right_child(), type(None)) and isinstance(tree2.get_right_child(), type(None)):
                if isinstance(self.get_left_child(), type(None)) and isinstance(tree2.get_left_child(), type(None)):
                    if self.get_root_val() == tree2.get_root_val():
                        return True
                    else:
                        return False
                else: #established right sides are both none
                    if isinstance(self.get_left_child(), type(None)) and isinstance(tree2.get_left_child(), type(None)):
                        return True
                    elif isinstance(self.get_left_child(), Binary_Tree) and isinstance(tree2.get_left_child(), type(None)):
                        return False
                    elif isinstance(self.get_left_child(), type(None)) and isinstance(tree2.get_left_child(), Binary_Tree):
                        return False
                    else:
                        return self.get_left_child().__eq__(tree2.get_left_child())   
            else: #established left sides are both none
                if isinstance(self.get_right_child(), type(None)) and isinstance(tree2.get_right_child(), type(None)):
                    return True
                elif isinstance(self.get_right_child(), type(None)) and isinstance(tree2.get_right_child(), Binary_Tree):
                    return False
                elif isinstance(self.get_right_child(), Binary_Tree) and isinstance(tree2.get_right_child(), type(None)):
                    return False
                else:
                    assert(isinstance(self.get_right_child(), Binary_Tree) and isinstance(tree2.get_right_child(), Binary_Tree))
                    return self.get_right_child().__eq__(tree2.get_right_child())

    def __repr__(self): #function for printing tree
        if self.is_leaf(): #base case for when we are at a leaf
            return "Binary_Tree(" + repr(self.get_root_val()) + ", None, None)"
        else: #now use recursion
            return "Binary_Tree(" + repr(self.get_root_val()) + ", " + repr(self.leftchild) + ", " + repr(self.rightchild) + ")"
        
'''
Below code implements the above object, adding in relevant questions.
'''

#if user answers no, go to left node; if user answer yes, go to right node
#below initializes animal-themed 20-questions tree

Tree1 = Binary_Tree("Is it a mammal?", "Does it live in the ocean?", "Does it fly?")
Tree1.get_left_child().add_left_child("Does it live in water?")
Tree1.get_left_child().get_left_child().add_left_child("Is it a snake?")
Tree1.get_left_child().get_left_child().add_right_child("Is it a crocodile?")
Tree1.get_left_child().add_right_child("Is it a predator?")
Tree1.get_left_child().get_right_child().add_left_child("Is it a turtle?")
Tree1.get_left_child().get_right_child().add_right_child("Is it a shark?")
Tree1.get_right_child().add_left_child("Does it have fur?")
Tree1.get_right_child().get_left_child().add_left_child("Is it an elephant?")
Tree1.get_right_child().get_left_child().add_right_child("Is it a lion?")
Tree1.get_right_child().add_right_child("Is it a predator?")
Tree1.get_right_child().get_right_child().add_right_child("Is it a hawk?")
Tree1.get_right_child().get_right_child().add_left_child("Is it a hummingbird?")
Final_Tree = Tree1

def Do_you_want_to_play(Tree1):
    yesno = raw_input("Do you want to play?")
    if yesno == "yes":
        raw_input("O.K., think of an animal!")
        Twenty_Questions(Tree1, Tree1)
    elif yesno == "no":
        print "o.k., bye!" + "\n" + "the final tree was:"
        print Final_Tree

def Twenty_Questions(smaller_tree, old_tree):
    if smaller_tree.is_leaf():
        my_guess = smaller_tree.get_root_val()
        answer = raw_input("Aha! " + my_guess)
        if answer == "yes":
            print "Cool! I guessed it!"
            Do_you_want_to_play(old_tree)
        else:
            print "Rats!"
            the_right_answer = raw_input("Tell me your animal: ")
            new_question = raw_input("Tell me a question that you would answer Yes for " + the_right_answer + " but no for " + my_guess + ":")
            #below modifies the wrong node by adding in new question
            smaller_tree.change_root_value(new_question)
            smaller_tree.add_left_child(my_guess)
            smaller_tree.add_right_child("Is it a " + the_right_answer + "?")
            Do_you_want_to_play(old_tree)
    else:
        question = raw_input(smaller_tree.get_root_val())
        if question == "yes":
            New_tree1 = smaller_tree.get_right_child()
            #question2 = raw_input(Tree)
            Twenty_Questions(New_tree1, Tree1)
        else:
            New_tree2 = smaller_tree.get_left_child()
            #question3 = raw_input(Tree)
            Twenty_Questions(New_tree2, Tree1)

Do_you_want_to_play(Tree1)
