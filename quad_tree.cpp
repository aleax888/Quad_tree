#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <climits>
#include <random>
using namespace std;

typedef pair<double, double> pdd;

/*generates a random number between two numbers that it receives as a
parameter, an upper limit and a lower limit*/
int generate_ramdon_number(int dawn, int up)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(dawn, up);
	return distrib(gen);
}

struct point
{
	//attributes
	pdd coordinate;

	//constructor
	point() // void constructor
	{
		coordinate.first = 0;
		coordinate.second = 0;
	}

	//point() //semi random constructor
	//{
	//	coordinate.first = generate_ramdon_number(0, 1000);
	//	coordinate.second = generate_ramdon_number(0, 1000);
	//}

	point(pdd coord) // constructor
	{
		coordinate.first = coord.first;
		coordinate.second = coord.second;
	}

	point(const point& p) // copy constructor
	{
		coordinate.first = p.coordinate.first;
		coordinate.second = p.coordinate.second;
	}

	//destructor
	/*~point()
	{
		delete[] coordinates;
	}*/

	//methods
	void copy(point p) // copies the coordinates of a point received as a parameter
	{
		coordinate.first = p.coordinate.first;
		coordinate.second = p.coordinate.second;
	}

	bool equal(point p) // return true if p1 and p2 are the same, else return false
	{
		if (this->coordinate.first != p.coordinate.first and this->coordinate.second != p.coordinate.second)
			return false;
		return true;
	}

	bool distinct(point p) // return false if p1 and p2 are the same, else return true
	{
		return !equal(p);
	}

	double euclidean_distance(point p) // return distance between two points
	{
		return sqrt(pow(this->coordinate.first - p.coordinate.first, 2) + pow(this->coordinate.second - p.coordinate.second, 2));
	}

	void printPoint() // print coordinates of the point
	{
		cout << "Point: " << coordinate.first << ", " << coordinate.second << endl;
	}
};

bool sortbysec(const point& a, const point& b);

struct quad_node
{
	//attributes
	pair<point, point> rectangle;
	vector<point> points;

	quad_node* childs[4];

	//constructors
	quad_node(point upper, point lower)
	{
		rectangle.first.copy(upper);
		rectangle.second.copy(lower);
	}

	quad_node(point upper, point lower, vector<point> p)
	{
		rectangle.first.copy(upper);
		rectangle.second.copy(lower);

		for (int i = 0, s = p.size(); i < s; ++i)
			points[i].copy(p[i]);

		for (int i = 0; i < 4; ++i)
			childs[i] == nullptr;
	}

	quad_node(const quad_node &qn)
	{
		rectangle.first.copy(qn.rectangle.first);
		rectangle.second.copy(qn.rectangle.second);

		for (int i = 0, s = qn.points.size(); i < s; ++i)
			points[i].copy(qn.points[i]);
	}

	quad_node()
	{
		
	}

	void insert(point p)
	{
		points.push_back(p);
		sort(points.begin(), points.end(), sortbysec);
	}

	void copy(quad_node qn)
	{
		rectangle.first.copy(qn.rectangle.first);
		rectangle.second.copy(qn.rectangle.second);

		for (int i = 0, s = qn.points.size(); i < s; ++i)
			points[i].copy(qn.points[i]);
	}

	bool equal(quad_node qn)
	{
		/*for (int i = 0, s = qn.points.size(); i < s; ++i)
			if (points[i].equal(qn.points[i]))
				return false;
		return true;*/
		return (qn.rectangle.first.equal(rectangle.first) and qn.rectangle.second.equal(rectangle.second));
	}

	void printQN()
	{
		cout << "rectangle:" << "\nupper: "; rectangle.first.printPoint(); 
		cout << "lower: "; rectangle.second.printPoint();

		cout << "points: " << endl;
		for (int i = 0, s = points.size(); i < s; ++i)
		{
			points[i].printPoint();
			cout << endl;
		}
		cout << endl;
	}
};

// function to change the sort order of the sort algorithm
bool sortbysec(const point& a, const point& b)
{
	return (a.coordinate.first < b.coordinate.first);
}


struct quad_tree
{
	int deep;
	quad_node *root;

	quad_tree(int d, point upper, point lower)
	{
		deep = d;
		root = new quad_node(upper, lower);

		quad_node **gen = &root;
		generate(gen, deep);
	}

	void in_orden(quad_node *aux)
	{
		if (aux)
		{
			in_orden(aux->childs[0]);
			in_orden(aux->childs[1]);
			aux->printQN();
			in_orden(aux->childs[2]);
			in_orden(aux->childs[3]);

		}
	}

	void generate(quad_node **gen, int d)
	{
		if (d > 1)
		{
			//(*gen)->printQN();
			int div_x = (*gen)->rectangle.second.coordinate.first - ((*gen)->rectangle.second.coordinate.first - (*gen)->rectangle.first.coordinate.first) / 2;
			int div_y = (*gen)->rectangle.first.coordinate.second - ((*gen)->rectangle.first.coordinate.second - (*gen)->rectangle.second.coordinate.second) / 2;
			(*gen)->childs[0] = new quad_node(point(pdd{ (*gen)->rectangle.first.coordinate.first, div_y }), point(pdd{ div_x, (*gen)->rectangle.second.coordinate.second }));
			(*gen)->childs[1] = new quad_node(point({ (*gen)->rectangle.first.coordinate.first, (*gen)->rectangle.first.coordinate.second }), point({ div_x, div_y }));
			(*gen)->childs[2] = new quad_node(point({ div_x,  (*gen)->rectangle.first.coordinate.second }), point({ (*gen)->rectangle.second.coordinate.first, div_y }));
			(*gen)->childs[3] = new quad_node(point({ div_x, div_y }), point({ (*gen)->rectangle.second.coordinate.first, (*gen)->rectangle.second.coordinate.second }));
			
			for (int i = 0; i < 4; ++i)
			{
				generate(&(*gen)->childs[i], d - 1);
			}
		}
	}

	bool inside(point p, quad_node qn)
	{
		/*cout << p.coordinate.first << " >= " << qn.rectangle.first.coordinate.first << endl;
		cout << p.coordinate.second << " <= " << qn.rectangle.first.coordinate.second << endl;
		cout << p.coordinate.first << " <= " << qn.rectangle.second.coordinate.first << endl;
		cout << p.coordinate.second << " >= " << qn.rectangle.second.coordinate.second << endl;*/
		if (p.coordinate.first >= qn.rectangle.first.coordinate.first and
			p.coordinate.second <= qn.rectangle.first.coordinate.second and
			p.coordinate.first <= qn.rectangle.second.coordinate.first and
			p.coordinate.second >= qn.rectangle.second.coordinate.second)
			return true;
		return false;
	}

	/*bool serch(point p, quad_node** &ptr)
	{
		if (!(*ptr)->childs[0] and !(*ptr)->childs[1] and !(*ptr)->childs[2] and !(*ptr)->childs[3])
		{
			for (int i = 0, s = (*ptr)->points.size(); i < s; ++i)
				if ((*ptr)->points[i].equal(p))
					return true;
			return false;
		}
		
		for (int i = 0; i < 4; ++i)
		{
			if (inside(p, *(*ptr)->childs[i]))
			{
				ptr = &(*ptr)->childs[i];
				serch(p, ptr);
				break;
			}
		}
	}

	

	void insert(point p)
	{
		quad_node** finder = &root;
		if (!serch(p, finder))
		{
			(*finder)->points.push_back(p);
		}
	}*/

	void ins(point p, quad_node** ptr)
	{
		for (int i = 0; i < 4; ++i)
		{
			if ((*ptr)->childs[i] and inside(p, *(*ptr)->childs[i]))
			{
				(*ptr)->childs[i]->points.push_back(p);
				ins(p, &(*ptr)->childs[i]);
			}
		}
		
	}

	void insert(point p)
	{
		quad_node** finder = &root;
		(*finder)->points.push_back(p);
		ins(p, finder);
	}
};

int main()
{
	point upper(pdd{ 0,40 });
	point lower(pdd{ 40,0 });
	quad_tree tree(2, upper, lower);

	tree.insert(point(pdd{ 20,20 }));
	tree.in_orden(tree.root);
}