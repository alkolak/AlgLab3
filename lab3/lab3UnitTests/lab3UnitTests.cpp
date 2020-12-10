#include "pch.h"
#include "CppUnitTest.h"
#include "..\lab3\lab3.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lab3UnitTests
{
	TEST_CLASS(lab3UnitTests)
	{
	public:
		
		TEST_METHOD(bft_Iterator_next)
		{
			BiniaryHeap B(1);
			Iterator* I = B.create_bft_iterator();
			
			Assert::AreEqual(1, I->next());
		}
		TEST_METHOD(bft_Iterator_has_next)
		{
			BiniaryHeap B(1);
			Iterator* I = B.create_bft_iterator();

			Assert::IsTrue(I->has_next());

			I->next();
			
			Assert::IsFalse(I->has_next());
		}
		TEST_METHOD(bft_Iterator_next_exception)
		{
			BiniaryHeap B(1);
			Iterator* I = B.create_bft_iterator();
			I->next();
			try
			{
				I->next();
				Assert::IsTrue(0);
			}
			catch (out_of_range& exception)
			{
				const char A[18] = "No more elements\n";
				Assert::AreEqual(exception.what(), A);
			}
			Assert::IsTrue(1);
		}

		TEST_METHOD(BiniaryHeap_insert)
		{
			BiniaryHeap B(1);
			Iterator* I;
			B.insert(0);
			I = B.create_bft_iterator();

			Assert::AreEqual(I->next(), 1);
			Assert::AreEqual(I->next(), 0);
			Assert::IsFalse(I->has_next());

			B.remove_iterators();
			B.insert(2);
			I = B.create_bft_iterator();

			Assert::AreEqual(I->next(), 2);
			Assert::AreEqual(I->next(), 0);
			Assert::AreEqual(I->next(), 1);
			Assert::IsFalse(I->has_next());
		}
		TEST_METHOD(BiniaryHeap_insert_resize)
		{
			BiniaryHeap B(1);
			Iterator* I;
			int i;
			for (i = 0;i < 9;i++)
				B.insert(1);
			B.insert(2);
			I = B.create_bft_iterator();

			Assert::AreEqual(I->next(), 2);
			for (i = 0;i < 10;i++)
				Assert::AreEqual(I->next(), 1);
			Assert::IsFalse(I->has_next());
		}
		TEST_METHOD(BiniaryHeap_insert_exception)
		{
			BiniaryHeap B(1);
			Iterator* I = B.create_bft_iterator();
			try
			{
				B.insert(2);
				Assert::IsTrue(0);
			}
			catch (runtime_error& exception)
			{
				const char A[27] = "Iterators must be removed\n";
				Assert::AreEqual(exception.what(), A);
			}
			Assert::IsTrue(1);
		}

		TEST_METHOD(dft_Iterator_next_has_next)
		{
			BiniaryHeap B(5);
			B.insert(4);
			B.insert(3);
			B.insert(2);
			B.insert(1);
			Iterator* I = B.create_dft_iterator();

			Assert::IsTrue(I->has_next());
			Assert::AreEqual(I->next(), 5);
			Assert::AreEqual(I->next(), 4);
			Assert::AreEqual(I->next(), 2);
			Assert::IsTrue(I->has_next());
			Assert::AreEqual(I->next(), 1);
			Assert::AreEqual(I->next(), 3);
			Assert::IsFalse(I->has_next());
		}
		TEST_METHOD(dft_Iterator_next_exception)
		{
			BiniaryHeap B(1);
			Iterator* I = B.create_dft_iterator();
			I->next();
			try
			{
				I->next();
				Assert::IsTrue(0);
			}
			catch (out_of_range& exception)
			{
				const char A[18] = "No more elements\n";
				Assert::AreEqual(exception.what(), A);
			}
			Assert::IsTrue(1);
		}

		TEST_METHOD(BiniaryHeap_contains)
		{
			BiniaryHeap B(1);
			B.insert(2);
			B.insert(3);

			Assert::IsTrue(B.contains(1));
			Assert::IsTrue(B.contains(3));
			Assert::IsFalse(B.contains(0));
		}
		TEST_METHOD(BiniaryHeap_contains_exception)
		{
			BiniaryHeap B(1);
			Iterator* I = B.create_bft_iterator();
			try
			{
				B.contains(1);
				Assert::IsTrue(0);
			}
			catch (runtime_error& exception)
			{
				const char A[27] = "Iterators must be removed\n";
				Assert::AreEqual(exception.what(), A);
			}
			Assert::IsTrue(1);
		}

		TEST_METHOD(BiniaryHeap_remove)
		{
			BiniaryHeap B(5);
			B.insert(3);
			B.insert(4);
			B.insert(2);
			B.insert(1);
			B.insert(4);
			
			B.remove(1);
			B.remove(5);

			Iterator* I = B.create_bft_iterator();

			Assert::AreEqual(I->next(), 4);
			Assert::AreEqual(I->next(), 3);
			Assert::AreEqual(I->next(), 4);
			Assert::AreEqual(I->next(), 2);
			Assert::IsFalse(I->has_next());
		}
		TEST_METHOD(BiniaryHeap_remove_exception1)
		{
			BiniaryHeap B(1);
			B.insert(2);
			B.insert(3);
			try
			{
				B.remove(0);
				Assert::IsTrue(0);
			}
			catch (invalid_argument& exception)
			{
				const char A[28] = "Heap isn't contain element\n";
				Assert::AreEqual(exception.what(), A);
			}
			Assert::IsTrue(1);
		}
		TEST_METHOD(BiniaryHeap_remove_exception2)
		{
			BiniaryHeap B(1);
			B.insert(2);
			Iterator* I = B.create_bft_iterator();
			try
			{
				B.remove(1);
				Assert::IsTrue(0);
			}
			catch (runtime_error& exception)
			{
				const char A[27] = "Iterators must be removed\n";
				Assert::AreEqual(exception.what(), A);
			}
			Assert::IsTrue(1);
		}
	};
}
