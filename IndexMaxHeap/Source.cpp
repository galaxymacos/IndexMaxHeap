#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;
template<typename Item>
class index_max_heap
{
	Item* data_{};
	int* indexes_{};
	int* reverse_{};
	int count_{};
	int capacity_{};

	void shift_up(int k)
	{
		while (k > 1&&data_[indexes_[k]]>data_[indexes_[k/2]])
		{
			swap(indexes_[k], indexes_[k / 2]);
			reverse_[indexes_[k]] = k;
			reverse_[indexes_[k / 2]] = k / 2;
			k /= 2;
		}
	}

	void shift_down(int k)
	{
		while(2*k<=count_)
		{
			int j = 2 * k;
			if(data_[indexes_[j+1]]>data_[indexes_[j]]&&j+1<=count_)
			{
				j++;
			}
			if (data_[indexes_[j]] <= data_[indexes_[k]])
				break;
			swap(indexes_[j], indexes_[k]);
			reverse_[indexes_[j]] = j;
			reverse_[indexes_[k]] = k;
			k = j;
		}
	}

public:
	explicit index_max_heap(const int capacity)
	{
		count_ = 0;
		indexes_ = new int[capacity + 1];
		capacity_ = capacity;
		data_ = new Item[capacity + 1];
		reverse_ = new int[capacity + 1];
	}

	~index_max_heap()
	{
		delete[] data_;
		delete[] indexes_;
		delete[] reverse_;
	}

	int size() const
	{
		return count_;
	}

	bool is_empty() const
	{
		return count_ == 0;
	}

	void insert(int i,Item item)
	{
		assert(count_ + 1 <= capacity_);	// 确保容量足够
		assert(i + 1 >= 1 && i + 1 <= capacity_);	// 确保元素的索引没有超过数组范围
		data_[++i] = item;
		indexes_[count_+1] = i;
		reverse_[i] = count_ + 1;
		count_++;
		shift_up(count_);
	}

	Item extract_max()
	{
		assert(count_ > 0);
		Item ret = data[indexes_[1]];
		swap(indexes_[1], indexes_[count_]);
		reverse_[indexes_[1]] = 1;
		reverse_[indexes_[count_]] = 0;
		count_--;
		shift_down(1);
		return ret;
	}

	int extract_max_index()
	{
		assert(count_ > 0);
		const auto ret = indexes_[1]-1;
		swap(indexes_[1], indexes_[count_]);
		reverse_[indexes_[1]] = 1;
		reverse_[indexes_[count_]] = 0;	// 已经删除的元素 在 indexes 数组中已经不存在了
		count_--;
		shift_down(1);
		return ret;
	}

	bool contain(const int i) const
	{
		assert(i + 1 >= 1 && i + 1 <= capacity_);
		return reverse_[i] != 0;
	}

	void change(int i,Item new_item)
	{
		assert(contain(i));
		i += 1;
		data[i] = new_item;
		// for (auto j = 1; i < count_; j++)
		// {
		// 	if (indexes_[j] == i)
		// 	{
		// 		shift_up(j);
		// 		shift_down(j);
		// 		return;
		// 	}
		// }
		const auto j = reverse_[i];
		shift_up(j);
		shift_down(j);
	}

	Item get_item(const int i)
	{
		assert(contain(i));
		return data_[i + 1];
	}
};