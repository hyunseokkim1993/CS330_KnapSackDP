#include "knapsack-dp.h"
#include <iostream>
#include <algorithm>

////////////////////////////////////////////////////////////
Item::Item( int const& weight, int const& value ) 
	: weight(weight), value(value) 
{
}

////////////////////////////////////////////////////////////
Item::Item( Item const& original ) 
	: weight(original.weight), value(original.value)
{
}

////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& os, Item const& item) {
	os << "(" << item.weight << " , " << item.value << ") ";
	return os;
}

////////////////////////////////////////////////////////////
std::istream& operator>> (std::istream& os, Item & item) {
	os >> item.weight >> item.value;
	return os;
}

////////////////////////////////////////////////////////////
typedef std::vector< std::vector<int> > Table; //2-dimensional table

////////////////////////////////////////////////////////////
//the returned value is a vector of indices
std::vector<int> knapsackDP( std::vector<Item> const& items, int const& W )
{
	int num_items = items.size();
	/*std::vector<std::vector<int> >*/ Table table(W+1, std::vector<int>(num_items+1, 0));
	////////

	for(int w = 1; w <=W; ++w)
	{
		for(int i =1; i <= num_items; ++i)
		{
			/*if (i == 0 || w == 0)
				table[w][i] = 0;*/
			if(items[i-1].weight <= w)
			{
				/*int a = items[i - 1].value + table[w - items[i - 1].weight][i - 1];
				int b = table[w][i-1];*/
				
				table[w][i]	= std::max( table[w - items[i - 1].weight][i - 1] + items[i - 1].value,
										table[w][i - 1]);
			}
			else
				table[w][i] = table[w][i-1];
		}
	}
    /*  ........... */

	//print final table - for debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { std::cout << items[n].weight << "," << items[n].value<<"   "; }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 


	//figure out which items are in the bag based on the table
	std::vector<int> bag;
    /*  ........... */
	int value = table[W][num_items];

	int weight = W;
	for (int i = num_items; i > 0 && value > 0; i--) 
	{
		if (value == table[weight][i-1])
			continue;
		else 
		{
			bag.push_back(i-1);

			value = value - items[i - 1].value;
			weight = weight - items[i - 1].weight;
		}
	}

	//delete[] table;
	return bag;
}

////////////////////////////////////////////////////////////
int valueBag( std::vector<Item> const& items, std::vector<int> const& bag ) {
	std::vector<int>::const_iterator it   = bag.begin(),
		                          it_e = bag.end();

	int accum = 0;
	//std::cout << "Bag ";
	for ( ; it != it_e; ++it) { 
		accum += items[ *it ].value; 
		//std::cout << *it << " ";
	}
	//std::cout << std::endl;
	return accum;
}

////////////////////////////////////////////////////////////
//prototype
//notice that auxiliary function returns value of the vector of items
//the actual vector is determined later from the table (similar to DP solution)
int knapsackRecMemAux( std::vector<Item> const&, int const&, int, int, int, Table& );

////////////////////////////////////////////////////////////
//function to kick start
std::vector<int> knapsackRecMem( std::vector<Item> const& items, int const& W )
{
	int num_items = items.size();

	//Allocate Table
	Table table(W + 1, std::vector<int>(num_items + 1));

	for (int w = 0; w <= W; ++w)
	{
		for(int i=0; i<=num_items; ++i)
		{
			if (w == 0 || i == 0)
				table[w][i] = 0;
			else
				table[w][i] = -1;
		}
	}
	////////
	table[W][num_items] = knapsackRecMemAux(items, W, num_items, W, num_items, table);
    /* ........... */
	
	//print table - debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { 
            std::cout << items[n].weight << "," << items[n].value<<"   "; 
        }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 

	//figure out which items are in the bag based on the table
	std::vector<int> bag;
	/*  ........... */
	int value = table[W][num_items];

	int weight = W;
	for (int i = num_items; i > 0 && value > 0; i--)
	{
		if (value == table[weight][i - 1])
			continue;
		else
		{
			bag.push_back(i - 1);

			value = value - items[i - 1].value;
			weight = weight - items[i - 1].weight;
		}
	}
	return bag;

}

////////////////////////////////////////////////////////////
//the real recursive function
int knapsackRecMemAux( std::vector<Item> const& items, int const& W, int num_items, int w, int i, Table & table )
{
	int value =0;

	if( table[w][i] < 0)
	{
		if(w < items[i - 1].weight)
		{
			value = knapsackRecMemAux(items, W, num_items, w, i-1, table);
		}
		else
		{
			value = std::max(knapsackRecMemAux(items, W, num_items,  w, i - 1, table),
				items[i - 1].value + knapsackRecMemAux(items, W, num_items,   w - items[i - 1].weight, i - 1, table) );
		}
		table[w][i] = value;
		
	}
	return table[w][i];
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
