
#Hashmap implementation (python)

'''
################################################TEST SUITE for hashmap
>>> new_map = hashmap()

>>> new_map.constructor(10)
[None, None, None, None, None, None, None, None, None, None]

>>> new_map.boolean_set('bob',1)
1

>>> new_map
[[('bob', 1)], None, None, None, None, None, None, None, None, None]

>>> new_map.boolean_set('bob', 1)
1

>>> new_map
[[('bob', 1)], None, None, None, None, None, None, None, None, None]

>>> new_map.get('bob')
1

>>> new_map.boolean_set('bob', 2)
1

>>> new_map
[[('bob', 2)], None, None, None, None, None, None, None, None, None]

>> new_map.set('bob')
2

>>> new_map.boolean_set('mary', 2)
1

>>> new_map
[[('bob', 2)], [('mary', 2)], None, None, None, None, None, None, None, None]

>>> new_map.delete('mary')
2

>>> new_map
[[('bob', 2)], [], None, None, None, None, None, None, None, None]

>>> new_map.delete('bobby')

>>> new_map
[[('bob', 2)], [], None, None, None, None, None, None, None, None]

>>> new_map.flat_load()
0.1

>>> new_map.num_items
1

>>> new_map.boolean_set('joe', 3)
1

>>> new_map.boolean_set('james', [1,2,3])
1

>>> new_map.boolean_set('sam', 'no')
1

>>> new_map.boolean_set('lee', 20)
1

>>> new_map.boolean_set('sara', 0)
1

>>> new_map.boolean_set('tom', {'cat':4})
1

>>> new_map.boolean_set('jess', 5)
1

>>> new_map.boolean_set('nigel', 8)
1

>>> new_map.boolean_set('miranda', 4)
1

>>> new_map.num_items
10

>>> new_map.boolean_set('josephine', 4)
0

>>> new_map.boolean_set('miranda', 5)
1

################################################
'''

class Node:
	def __init__(self, tup):
		self.data = tup #tup is (key, value)
		self.next = None #allows nodes to point to each other
	def set_data(self, new_tup):
		self.data = new_tup #allows us to mutate the tuple
	def get_data(self):
		return self.data
	def set_next(self, next_item):
		self.next = next_item
	def get_next(self):
		return self.next
	def __repr__(self):
		return repr(self.data)

class LinkedList: #uses node class to create a linked list of nodes
	def __init__(self):
		self.top = None
	def add_element(self, element): #element is a tuple of form (string, data)
		#if key already exists, overwrite value with element[1] using set_data
		curr_element = self.top
		while curr_element != None:
			if curr_element.data[0] == element[0]: #if key already in LL
				curr_element.set_data(element) #change value to new value
				return 1 #if success
			curr_element = curr_element.next

		#if key of element is not in the nodes, create a new node with element
		n = Node(element) #create new node if key is not in LL
		n.set_next(self.top) #self.top is still the previous linked list, will be moved to the pointer of n
		self.top = n #self.top reset to n

	def get_value(self, key): #self is LL
		curr_element = self.top #first element in LL
		while curr_element != None:
			if curr_element.get_data()[0] == key:
				return curr_element.get_data()[1]
			else:
				curr_element = curr_element.get_next()
		return None

	def delete_value(self, key):
		previous_element = None
		curr_element = self.top

		while curr_element != None:
			if curr_element.data[0] == key:
				if previous_element == None:
					self.top = curr_element.get_next()
					return 1
				else:
					previous_element.set_next(curr_element.get_next())
					return 1
			else:
				previous_element = curr_element
				curr_element = curr_element.get_next()
				return 1
		
		return 0 #returns 0 if we never find the value associated with the key or if process does not work

	def __repr__(self):
		array = []
		curr_element = self.top
		while curr_element != None:
			array.append((curr_element))
			curr_element = curr_element.get_next()
		return str(array)



###################################################

class hashmap: #implements both Node and LinkedList
	def __init__(self):
		self.size = 0
		self.num_items = 0
		self.map = [] #array that will hold hashmap pairs

	def constructor(self, size): #return an instance of the class with pre-allocated space for the given number of objects
		self.size = size
		self.map = [None]*size 
		return self.map

	def boolean_set(self, key, value): #stores the key and value in the map and returns True or False if the operation succeeds
		#key is a string
		new_hash = hash(key)
		index = new_hash%self.size

		if self.num_items < self.size: #any add is fine
			if self.map[index] == None: #no collision
				LL = LinkedList()
				LL.add_element((key, value))
				self.num_items += 1
				self.map[index] = LL
				return 1
			else: #collision! just add key, value pair to linked list
				assert(self.map[index]!= None)
				LL = self.map[index]
				if LL.get_value(key) == None:
					LL.add_element((key, value))
					self.num_items += 1
					return 1
				else:
					LL.add_element((key, value))
					return 1

		elif self.num_items == self.size: #only adds when keys already exist
			if self.map[index] == None:
				return 0 #not possible
			else:
				LL = self.map[index]
				if LL.get_value(key) == None:
					return 0
				else:
					LL.add_element((key, value))
					return 1
		else:
			return 0

	def get(self, key): #returns value associated with the key or None if no value exists
		index = hash(key)%self.size
		if self.map[index] == None:
			return None
		else:
			LL = self.map[index]
			return LL.get_value(key)

	def delete(self, key): #deletes the value associated with the given key, returns the value if successful
		index = hash(key)%self.size
		if self.map[index]!= None:
			LL = self.map[index]
			if LL.get_value(key) != None:
				value = LL.get_value(key)
				LL.delete_value(key)
				self.num_items = self.num_items - 1
				return value
			else:
				return None
		else: #if key has no value
			assert(self.map[index] == None)
			return None

	def flat_load(self): #returns (items in hash map)/(size of hash map) - should never be greater than 1
		return float(self.num_items)/self.size

	def __repr__(self):
		array = []
		index = 0
		while index < len(self.map):
			curr_list = self.map[index]
			array.append(curr_list)
			index += 1
		return str(array)

if __name__ == "__main__": #runs doctests at the top of the file
    import doctest
    doctest.testmod()


