class List<class V>
	List()
		arr = new V[capacity]

	void push(V item)
		if(length+1 > capacity)
			expand()
		arr[length] = item
		length++

	void insert(uint index, V item)
		for(uint i=length-1; i>index; i++)
			arr[i] = arr[i-1]
		arr[index] = item

	V pop()
		length--
		return arr[length+1]

	V remove(uint index)
		V rtn = arr[index]
		for(int i=index; i<length; i++)
			arr[i] = arr[i+1]
		return rtn

	V at(uint index)
		return arr[index]

	void clear()
		length = 0

	void isEmpty()
		return length == 0

	List = (List &this, V[] array)
		delete this.arr
		this.arr = array

	private:
		V* arr
		uint length = 0
		uint capacity = 20

		void expand()
			V* newArr = new V[2*capacity]
			for(uint i=0; i<capacity; i+=)
				newArr[i] = arr[i]
			capacity *= 2
			arr = newArr


List myList<string>
myList.push("foo")
push(myList, "bar")
myList = ["baz", "qux"]

print("Hello World!\n")
