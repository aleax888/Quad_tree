#include <iostream>
#include <utility>
#include <vector>
using namespace std;

typedef pair<double, double> pdd;

struct point
{
    pdd coordinate;

    point()
    {
        coordinate.first = 0;
        coordinate.second = 0;
    }

    point(const point& p)
    {
        this->coordinate = p.coordinate;
    }

    point(double x, double y)
    {
        this->coordinate.first = x;
        this->coordinate.second = y;
    }

    point(pdd coord)
    {
        this->coordinate.first = coord.first;
        this->coordinate.second = coord.second;
    }

    void copy(point p)
    {
        this->coordinate = p.coordinate;
    }

    bool equal(point p)
    {
        return (p.coordinate.first == this->coordinate.first and p.coordinate.second == this->coordinate.second);
    }

    void print_point()
    {
        cout << "point: " << this->coordinate.first << ", " << this->coordinate.second << endl;
    }
};

struct quad_node
{
    point* rectangle[2];
    vector<point> points;
    quad_node* childs[4];

    quad_node()
    {
        rectangle[0] = new point();
        rectangle[1] = new point();

        for (int i = 0; i < 4; ++i)
            childs[i] = nullptr;
    }

    quad_node(const quad_node &qn)
    {
        this->rectangle[0] = new point(*qn.rectangle[0]);
        this->rectangle[1] = new point(*qn.rectangle[1]);
        this->points = qn.points;

        for (int i = 0; i < 4; ++i)
            childs[i] = nullptr;
    }

    quad_node(point upper, point lower)
    {
        this->rectangle[0] = new point(upper);
        this->rectangle[1] = new point(lower);

        for (int i = 0; i < 4; ++i)
            childs[i] = nullptr;
    }

    quad_node(point upper, point lower, vector<point> ps)
    {
        this->rectangle[0] = new point(upper);
        this->rectangle[1] = new point(lower);
        this->points = ps;

        for (int i = 0; i < 4; ++i)
            childs[i] = nullptr;
    }

    void insert(point p)
    {
        this->points.push_back(p);
    }

    void copy(quad_node qn)
    {
        this->rectangle[0] = new point(*qn.rectangle[0]);
        this->rectangle[1] = new point(*qn.rectangle[1]);
        this->points = qn.points;
    }

    void print_qn()
    {
        cout << "rectangle: " << endl;
        cout << "\t"; this->rectangle[0]->print_point();
        cout << "\t"; this->rectangle[1]->print_point();
        cout << "points: " << endl;
        for (int i = 0, s = points.size(); i < s; ++i)
        {
            cout << "\t"; points[i].print_point();
        }
    }
};

struct quad_tree
{
    int max_deep;
    quad_node* root;

    quad_tree()
    {
        this->max_deep = 0;
        this->root = new quad_node(point(), point());
    }

    quad_tree(int d, point upper, point lower)
    {
        this->max_deep = d;
        this->root = new quad_node(upper, lower);

        quad_node** gen = &root;
        generate(gen, max_deep);
    }

    void in_orden(quad_node* aux)
    {
        if (aux)
        {
            in_orden(aux->childs[0]);
            in_orden(aux->childs[1]);
            aux->print_qn();
            in_orden(aux->childs[2]);
            in_orden(aux->childs[3]);
        }
    }

    void generate(quad_node** gen, int d)
    {
        if (d > 1)
        {
            double div_x = (*gen)->rectangle[1]->coordinate.first  - ((*gen)->rectangle[1]->coordinate.first  - (*gen)->rectangle[0]->coordinate.first)  / 2;
            double div_y = (*gen)->rectangle[0]->coordinate.second - ((*gen)->rectangle[0]->coordinate.second - (*gen)->rectangle[1]->coordinate.second) / 2;
            (*gen)->childs[0] = new quad_node(point(pdd{ (*gen)->rectangle[0]->coordinate.first, div_y }), point(pdd{ div_x, (*gen)->rectangle[1]->coordinate.second }));
            (*gen)->childs[1] = new quad_node(point({ (*gen)->rectangle[0]->coordinate.first, (*gen)->rectangle[0]->coordinate.second }), point({ div_x, div_y }));
            (*gen)->childs[2] = new quad_node(point({ div_x,  (*gen)->rectangle[0]->coordinate.second }), point({ (*gen)->rectangle[1]->coordinate.first, div_y }));
            (*gen)->childs[3] = new quad_node(point({ div_x, div_y }), point({ (*gen)->rectangle[1]->coordinate.first, (*gen)->rectangle[1]->coordinate.second }));

            for (int i = 0; i < 4; ++i)
            {
                generate(&(*gen)->childs[i], d - 1);
            }
        }
    }

    bool inside(point p, quad_node qn)
    {
        if (p.coordinate.first >= qn.rectangle[0]->coordinate.first and p.coordinate.second <= qn.rectangle[0]->coordinate.second and p.coordinate.first <= qn.rectangle[1]->coordinate.first and p.coordinate.second >= qn.rectangle[1]->coordinate.second)
            return true;
        return false;
    }

    void aux_insert(point p, quad_node** ptr)
    {
        for (int i = 0; i < 4; ++i)
        {
            if ((*ptr)->childs[i] and inside(p, *(*ptr)->childs[i]))
            {
                (*ptr)->childs[i]->points.push_back(p);
                aux_insert(p, &(*ptr)->childs[i]);
            }
        }
    }

    void insert(point p)
    {
        quad_node** finder = &root;
        (*finder)->points.push_back(p);
        aux_insert(p, finder);
    }
};



int main()
{
    point upper(0, 40), lower(40, 0), a(upper);
    
    quad_tree tree(2, upper, lower);

    tree.insert(point(1, 2));
    tree.insert(point(2, 2));
    tree.insert(point(39, 39));
    tree.insert(point(15, 25));

    tree.in_orden(tree.root);
}
