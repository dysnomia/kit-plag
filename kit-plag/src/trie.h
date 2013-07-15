#pragma once
#ifndef TRIE_H
#define TRIE_H

#include "common.h"

/*
 * Trie that stores generic stuff at its tips.
 *   T needs to be default constructible.
 */
template <typename T>
class Trie {
private:
	/*
	 * Types: Trie nodes and links between tries.
	 */
	struct Node;
	typedef std::shared_ptr<Node> NodePtr;
	struct Link {
		char label;
		NodePtr child;

		Link(char label, NodePtr child) : label(label), child(child) {
		}
		bool operator<(const Link& other) const {
			return label < other.label;
		}
		operator char() const {
			return label;
		}
	};
	struct Node {
		T content;
		bool is_tip;
		std::vector<Link> links;

		Node() : content(), is_tip(false), links() {
		}
	};
	NodePtr root;

public:
	/*
	 * Constructors: default and move constructors.
	 */
	Trie() : root(new_node()) {
	}
	Trie(Trie&& other) : root(std::move(other.root)) {
		other.root = new_node();
	}
	Trie& operator=(Trie&& other) {
		root = std::move(other.root);
		return *this;
	}

	/*
	 * Element insertion (non-const), find (const) and containment check.
	 */
	T& operator[](const std::string& str) {
		NodePtr node = find_node(root, str, 0, str.size(), true);
		node->is_tip = true;
		return node->content;
	}
	const T& operator[](const std::string& str) const {
		NodePtr node = find_node_const(root, str, 0, str.size());
		if (!node || !node->is_tip) {
			throw std::invalid_argument(str + " not found in Trie");
		}
		return node->content;
	}
	bool has(const std::string& str) const {
		NodePtr node = find_node_const(root, str, 0, str.size());
		return node && node->is_tip;
	}

	/*
	 * Forward iterator. Invalidation: always...
	 */
	class iterator {
	public:
		typedef std::pair<std::string, T&> value_type;
	private:
		friend class Trie;

		// Position in trie (as stack)
		typedef std::stack<std::pair<NodePtr, size_t>> State;
		State state;
		// Current string position
		std::string prefix;

		iterator(NodePtr root) : state() {
			state.push(make_pair(root, 0));
			if (! root->is_tip) {
				incr();
			}
		}

		/* Increment iterator: quite difficult */
		void incr() {
			if (! state.empty()) {
				/* Go up as far as necessary */
				auto *top = &state.top();
				if (top->first->links.size() == 0) {
					while (top->second + 1 >= top->first->links.size()) {
						state.pop();
						if (state.empty()) {
							return;
						}
						prefix.pop_back();
						top = &state.top();
					}
					top->second++;
				}

				/* Go down until we get to a tip */
				do  {
					Link& down = top->first->links[top->second];
					state.push(make_pair(down.child, 0));
					prefix.push_back(down.label);
					top = &state.top();
				} while (! top->first->is_tip);
			}
		}

		/* Do not allow operator-> since operator* creates an rvalue. */
		value_type* operator->() const {
			return &(this->operator*());
		}

	public:
		/*
		 * Common forward iterator stuff.
		 */
		iterator() : state(), prefix() {
		}
		bool operator==(const iterator& other) const {
			return state == other.state;
		}
		bool operator!=(const iterator& other) const {
			return ! this->operator==(other);
		}
		const value_type operator*() const {
			if (state.empty()) {
				throw std::range_error("iterator not dereferenceable");
			}
			return value_type(prefix, state.top().first->content);
		}
		iterator& operator++() { // prefix
			incr();
			return *this;
		}
		iterator operator++(int) { // postfix
			iterator old(*this);
			incr();
			return old;
		}
	};

	/* HACK: const-guarantee not necessarily fulfilled */
	iterator begin() const {
		return iterator(root);
	}
	iterator end() const {
		return iterator();
	}

private:
	/*
	 * Search for str[idx .. len - 1] in the trie rooted at node...
	 *   create: create nodes along the way?
	 *   create = false -> const guarantee (not explicitly in language)
	 */
	NodePtr find_node(NodePtr trie, const std::string& str, size_t idx, size_t len, bool create) {
		if (idx >= len) {
			return trie;
		} else {
			auto &links = trie->links;
			char cur = str[idx];
			auto it = std::lower_bound(std::begin(links), std::end(links), cur);

			/* Not found: insert or return nullptr */
			if (it == std::end(links) || it->label != cur) {
				if (create) {
					it = links.insert(it, Link(cur, new_node()));
				} else {
					return nullptr;
				}
			}

			return find_node(it->child, str, idx + 1, len, create);
		}
	}

	/* Hack: find_node is constant when called with false -> wrap it into const_cast to avoid code duplication */
	NodePtr find_node_const(NodePtr trie, const std::string& str, size_t idx, size_t len) const {
		Trie* non_const = const_cast<Trie*>(this);
		return non_const->find_node(root, str, idx, len, false);
	}

	/* Creates an empty node on the heap */
	NodePtr new_node() const {
		return std::make_shared<Node>();
	}

	/* Disallow deep copying */
	Trie(const Trie& other);
	Trie& operator=(const Trie& other);
};

#endif
