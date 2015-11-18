
class List<class V>
	List.List()
		arr = new V[capacity]
	void push(V item)
	void insert(uint index, V item)
	V pop()
	V remove(uint index)
	V at(uint index)
		return arr[index]

	void clear()
		length = 0

	void isEmpty()
		return length == 0

	List = (List &this, V[] array)

	private:
		V* arr
		uint length = 0
		uint capacity = 20
		void expand()
