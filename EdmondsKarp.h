#pragma once
#include <string.h>
#include  <time.h>
#include  <conio.h>
#include  <iostream>
#include  <malloc.h>
#include  <fstream>

using namespace std;

int str_to_int(char* s)
{
	if (s == NULL)
		return NULL;

	if (*s == '\0')
		return NULL;

	int i = 0,
		n = 0,
		a = 0,
		sign;

	if (*s == '-')
	{
		sign = -1;
		i++;
	}
	else
		sign = 1;

	int flag = 1;

	do
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			n *= 10;
			a = ((int)s[i]) - 48;
			n += a;
			i++;
		}
		else
		{
			n = NULL;
			flag = 0;
		}
	} while (s[i] != '\0' && flag);

	n *= sign;
	return n;
}

struct Edge;

class Node
{
private:
public:	char name;
		Edge* sons;
		int number_sons;
		Node(char n_name) { name = n_name; sons = nullptr; number_sons = 0; };
		~Node() { if (sons) free(sons); };
};

struct Edge
{
	Node* begin;
	Node* end;
	int bandwidth;
	int flow;
};

struct shortest_way
{
	Node* node;
	Edge* way_to_parent;
	shortest_way* parent;
};

struct shortest_way_list
{
	shortest_way* part;	//way from start to node
	shortest_way_list* next;
};

class Web
{
private:
	Node* start;
	Node* finish;
	Edge** edges;
	int number_edges;
	bool calculated;

	void res_show_in_width(Node* current)
	{
		for (int i = 0; i < current->number_sons; i++)
		{
			cout << current->name << (current->sons[i]).end->name << " " << (current->sons[i]).flow << "/" << (current->sons[i]).bandwidth << ";  "/*<< endl*/;
			res_show_in_width(current->sons[i].end);
		}
	};

	shortest_way* shortest(Node* start, Node* finish)
	{
		if (start && finish)
		{
			shortest_way *result = new shortest_way;	//start point
			result->node = start;
			result->parent = nullptr;
			result->way_to_parent = nullptr;

			shortest_way_list list;			//make a list-node to contain start node
			shortest_way_list* tail = &list;
			list.part = result;
			list.next = nullptr;

			shortest_way_list* current = &list;

			bool found = 0;			//finish node is found
			while (current && !found)
			{
				if (current->part->node == finish) found = 1;
				else								//if not finish node
				{
					for (int i = 0; i < current->part->node->number_sons; i++)	//for all sons of current node
					{
						if (current->part->node->sons[i].bandwidth - current->part->node->sons[i].flow > 0)	//if the flow is not max
						{
							tail->next = new shortest_way_list;	//add to the end of list
							tail = tail->next;
							tail->next = nullptr;
							tail->part = new shortest_way;

							tail->part->parent = current->part;	//the way back from tail is current 
							tail->part->node = current->part->node->sons[i].end;
							tail->part->way_to_parent = &(current->part->node->sons[i]);
						}
					}
					current = current->next;
				}
			}
			if (found) { calculated = 1; return current->part; }
			else return nullptr;
		}
		else throw invalid_argument("Can't find shorthest in empty");
	};

public:
	Web()
	{
		start = nullptr;
		finish = nullptr;
		edges = nullptr;
		number_edges = 0;
		calculated = 0;
	};

	~Web()
	{
		if (start) free(start);
		if (edges) 	free(edges);
	}

	void add_edge(char n_begin, char n_end, int n_bandwidth)
	{
		if ((n_begin != n_end) && n_bandwidth > 0)
		{
			bool create = 1;
			Node* from = nullptr;
			Node* to = nullptr;

			for (int i = 0; i < number_edges && create; i++)	//find out if nodes with this name exist
			{
				if (edges[i]->begin->name == n_begin && edges[i]->end->name == n_end && edges[i]->bandwidth == n_bandwidth) create = 0;

				if (edges[i]->begin->name == n_begin) from = edges[i]->begin;
				if (edges[i]->end->name == n_begin) from = edges[i]->end;

				if (edges[i]->begin->name == n_end) to = edges[i]->begin;
				if (edges[i]->end->name == n_end) to = edges[i]->end;
			}

			if (create)
			{
				if (!from) from = new Node(n_begin);
				if (!to) to = new Node(n_end);

				if (n_begin == 's') start = from;
				if (n_end == 't') finish = to;

				from->sons = (Edge*)realloc(from->sons, (from->number_sons + 1) * sizeof(Edge));
				from->sons[from->number_sons].begin = from;
				from->sons[from->number_sons].end = to;
				from->sons[from->number_sons].bandwidth = n_bandwidth;
				from->sons[from->number_sons].flow = 0;

				edges = (Edge**)realloc(edges, (number_edges + 1) * sizeof(Edge*));
				edges[number_edges] = &(from->sons[from->number_sons]);

				number_edges++;
				from->number_sons++;
			}
			else
				throw invalid_argument("Trying add an existing edge");
		}
		else
		{
			if (n_begin == n_end) throw invalid_argument("Trying to add loop edge");
			else throw invalid_argument("Trying to add edge with bandwidth less 1");
		}
	}

	void EdmondsKarp()
	{
		shortest_way* path = shortest(start, finish);	//find a shortest way from start to finish

		if (path)
			while (path)
			{
				shortest_way* current = path;
				int min_bandwidth = current->way_to_parent->bandwidth;

				while (current)	//find the min bandwidth of edges (current) in path
				{
					if (current->way_to_parent)
						if (current->way_to_parent->bandwidth - current->way_to_parent->flow < min_bandwidth) min_bandwidth = current->way_to_parent->bandwidth - current->way_to_parent->flow;
					current = current->parent;
				}

				current = path;
				while (current)
				{
					if (current->way_to_parent)
						current->way_to_parent->flow += min_bandwidth;
					current = current->parent;
				}
				path = shortest(start, finish);
			}
		else throw runtime_error("Can't find way from start to finish");
	};

	void show_in_width()
	{
		if (true) //calculated
		{
			res_show_in_width(start);
			cout << endl;
		}
		else
			throw runtime_error("The result is not calculated");
	};

	void show_edges()
	{
		for (int i = 0; i < number_edges; i++)
			cout << edges[i]->begin->name << edges[i]->end->name << " " << edges[i]->flow << "/" << edges[i]->bandwidth << ";  " /*<< endl*/;
		cout << endl;
	};

	void show_shortest(shortest_way* current)
	{
		if (current)
		{
			cout << current->node->name;
			if (current->node->name != 's') cout << current->way_to_parent->flow << "/" << current->way_to_parent->bandwidth << " ";
			show_shortest(current->parent);
		}
	};

	int summ_flow()
	{
		if (start)
		{
			int res = 0;
			for (int i = 0; i < start->number_sons; i++)
				res += start->sons[i].flow;
			return res;
		}
		else throw runtime_error("No start node to count flow");
	};

	void from_file()
	{
		char buff[160];

		std::ifstream fin;
		fin.open("text.txt");
		if (!fin)
			throw runtime_error("File not found");
		//go to the beginning of the file
		fin.seekg(fin.beg);
		while (!fin.eof())
		{
			fin.getline(buff, 160);
			if (buff[0])
			{
				char out = 0;
				char in = 0;
				int bandwidth = 0;
				for (int i = 0; i < strlen(buff); i++)
				{
					if (buff[i] != ' ' && out == 0) out = buff[i];		//set start name
					else
						if (buff[i] != ' ' && in == 0) in = buff[i];	//set fiish name
						else
							if (buff[i] != ' ')							//set bandwidth
							{
								int j = i;
								for (; j < strlen(buff); j++);
								char* to_int = (char*)malloc((1 + j - i) * sizeof(char));
								for (int n = 0; n < j - i; n++)
									to_int[n] = buff[i + n];
								to_int[j-i] = 0;
								add_edge(out, in, str_to_int(to_int));
							}
				}
			}
		}
		fin.close();
	}
};
