//#include "gtest/gtest.h"

class Environment final : public ::testing::Environment
{
public:
	~Environment() override = default;

	// Override this to define how to set up the environment.
	void SetUp() override
	{
		//  initialize random seed
		srand(time(nullptr));
	}

	// Override this to define how to tear down the environment.
	void TearDown() override {}
};

// create our test class to house shared data between tests
// you should not need to change anything here
class CollectionTest : public ::testing::Test
{
protected:
	// create a smart point to hold our collection
	std::unique_ptr<std::vector<int>> collection;

	void SetUp() override
	{
		// create a new collection to be used in the test
		collection.reset(new std::vector<int>);
	}

	void TearDown() override
	{
		//  erase all elements in the collection, if any remain
		collection->clear();
		// free the pointer
		collection.reset(nullptr);
	}

	// helper function to add random values from 0 to 99 count times to the collection
	void add_entries(const int count) const
	{
		assert(count > 0);
		for (auto i = 0; i < count; ++i)
		{
			collection->push_back(rand() % 100);
		}
	}
}; // end of CollectionTest

// Uncomment FAIL(); to fail this test
TEST_F(CollectionTest, AlwaysFail)
{
	//FAIL();
	SUCCEED();
}

TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
	// is the collection created
	ASSERT_TRUE(collection);

	// if empty, the size must be 0
	ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
	// is the collection empty?
	ASSERT_TRUE(collection->empty());

	// if empty, the size must be 0
	ASSERT_EQ(collection->size(), 0);
}

//============================================================== todo_list ========

// Create a test to verify adding a single value to an empty collection
TEST_F(CollectionTest, CanAddToEmptyVector)
{
	// is the collection empty?
	// if empty, the size must be 0
	ASSERT_TRUE(collection->empty());
	add_entries(1);
	ASSERT_EQ(collection->size(), 1);
	// is the collection still empty?
	// if not empty, what must the size be?
}

// Create a test to verify adding five values to collection
TEST_F(CollectionTest, CanAddFiveValuesToVect)
{
	ASSERT_TRUE(collection->empty());
	add_entries(5);
	ASSERT_TRUE(collection.get()->size() >= 5);
}

// Verify clear erases collection
TEST_F(CollectionTest, ClearCollectionValues)
{
	add_entries(2);
	collection->clear();
	ASSERT_EQ(collection->size(), 0);
}

// Verify erase(begin,end) erases collection
TEST_F(CollectionTest, EraseCollection)
{
	add_entries(10);
	collection->erase(collection->begin(), collection->end());
	ASSERT_EQ(collection->size(), 0);
}

// Verify resizing increases collection
TEST_F(CollectionTest, increaseCollectionSize)
{
	add_entries(10);
	collection->resize(15);
	ASSERT_TRUE(collection.get()->size() >= 10);
}

// Verify reserve increases the capacity but not the size of collection
//  resize increases and initializes values as 0
TEST_F(CollectionTest, testReserveVectorCapacity)
{
	add_entries(5);
	collection->resize(10);
	ASSERT_EQ(collection->at(7), 0);
	ASSERT_EQ(collection.get()->size(), 10);
}

// Resizing to 0 decreases collection to zero
TEST_F(CollectionTest, ResizeToZero)
{
	add_entries(15);
	collection->resize(0);
	ASSERT_EQ(collection->size(), 0);
}

// Create a negative test to verify the std::out_of_range exception is thrown when calling at()
// with an index out of bounds
TEST_F(CollectionTest, TestOutOfRange)
{
	ASSERT_THROW(collection->at(5), std::out_of_range);
}

// Check for values above 100 (since random numbers are generated with 'mod 100')
TEST_F(CollectionTest, MaxSizeValues)
{
	// No value should be above 100.
	add_entries(10);
	for (auto i : *collection)
	{
		EXPECT_LT(i, 100);
	}
}

// Verify capacity is >= size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, verifyCapacityValues)
{

	EXPECT_EQ(collection->capacity(), 0);

	add_entries(1);
	EXPECT_EQ(collection->capacity(), 1);

	add_entries(3);
	EXPECT_EQ(collection->capacity(), 4);

	add_entries(5);
	EXPECT_EQ(collection->capacity(), 9);
}

// Positive Custom Test on collection
TEST_F(CollectionTest, AssertNoThrow)
{
	add_entries(1);
	ASSERT_NO_THROW(collection.get()->at(0));
}

// Negative Custom Test on collection
TEST_F(CollectionTest, AssertAnyThrow)
{
	ASSERT_ANY_THROW(collection.get()->at(0));
}
