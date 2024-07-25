#include "rb_tree.h"

#include <src/library/testing/unittest/registar.h>

#include <src/util/random/fast.h>
#include <src/util/random/easy.h>
#include <src/util/random/shuffle.h>

class TRedBlackTreeTest: public TTestBase {
    struct TCmp {
        template <class T>
        static inline bool Compare(const T& l, const T& r) {
            return l.N < r.N;
        }

        template <class T>
        static inline bool Compare(const T& l, int r) {
            return l.N < r;
        }

        template <class T>
        static inline bool Compare(int l, const T& r) {
            return l < r.N;
        }
    };

    class TNode: public TRbTreeItem<TNode, TCmp> {
    public:
        inline TNode(int n) noexcept
            : N(n)
        {
        }

        int N;
    };

    using TTree = TRbTree<TNode, TCmp>;

    UNIT_TEST_SUITE(TRedBlackTreeTest);
    UNIT_TEST(TestEmpty)
    UNIT_TEST(TestInsert)
    UNIT_TEST(TestErase)
    UNIT_TEST(TestFind)
    UNIT_TEST(TestStress)
    UNIT_TEST(TestGettingIndexWithDifferentValues)
    UNIT_TEST(TestCheckChildrenAfterErase)
    UNIT_TEST(TestGettingIndexWithDifferentValuesAfterErase)
    UNIT_TEST(TestGettingIndexWithEqualValues)
    UNIT_TEST(TestLessCountOnEmptyTree)
    UNIT_TEST_SUITE_END();

private:
    inline void TestStress() {
        std::vector<std::shared_ptr<TNode>> nodes;

        for (int i = 0; i < 1000; ++i) {
            nodes.push_back(std::make_shared<TNode>(i));
        }

        TTree tree;
        TReallyFastRng32 rnd(Random());

        for (size_t i = 0; i < 1000000; ++i) {
            tree.Insert(nodes[rnd.Uniform(nodes.size())].get());
        }

        for (TTree::TConstIterator it = tree.Begin(); it != tree.End();) {
            const int v1 = it->N;

            if (++it == tree.End()) {
                break;
            }

            const int v2 = it->N;

            UNIT_ASSERT(v1 < v2);
        }
    }

    inline void TestGettingIndexWithDifferentValues() {
        std::vector<std::shared_ptr<TNode>> nodes;
        size_t N = 1000;

        for (size_t i = 0; i < N; ++i) {
            nodes.push_back(std::make_shared<TNode>(int(i)));
        }

        TTree tree;
        Shuffle(nodes.begin(), nodes.end());

        for (size_t i = 0; i < N; ++i) {
            tree.Insert(nodes[i].get());
        }

        for (size_t i = 0; i < N; ++i) {
            UNIT_ASSERT_EQUAL(tree.LessCount(i), i);
            UNIT_ASSERT_EQUAL(tree.NotGreaterCount(i), i + 1);
            UNIT_ASSERT_EQUAL(tree.GreaterCount(i), N - i - 1);
            UNIT_ASSERT_EQUAL(tree.NotLessCount(i), N - i);

            auto nodePtr = tree.Find(i);
            UNIT_ASSERT_EQUAL(tree.GetIndex(nodePtr), i);
            UNIT_ASSERT_EQUAL(tree.GetIndex(nodes[i].get()), static_cast<size_t>(nodes[i]->N));
        }
    }

    inline void TestCheckChildrenAfterErase() {
        std::vector<std::shared_ptr<TNode>> nodes;
        size_t N = 1000;

        for (size_t i = 0; i < N; ++i) {
            nodes.push_back(std::make_shared<TNode>(int(i)));
        }

        TTree tree;
        Shuffle(nodes.begin(), nodes.end());

        for (size_t i = 0; i < N; ++i) {
            tree.Insert(nodes[i].get());
        }
        auto checker = [](const TTree& tree) {
            for (auto node = tree.Begin(); node != tree.End(); ++node) {
                size_t childrens = 1;
                if (node->Left_) {
                    childrens += node->Left_->Children_;
                }
                if (node->Right_) {
                    childrens += node->Right_->Children_;
                }
                UNIT_ASSERT_VALUES_EQUAL(childrens, node->Children_);
            }
        };

        for (auto node : nodes) {
            tree.Erase(node.get());
            checker(tree);
        }
    }

    inline void TestGettingIndexWithDifferentValuesAfterErase() {
        std::vector<std::shared_ptr<TNode>> nodes;
        size_t N = 1000;

        for (size_t i = 0; i < N; ++i) {
            nodes.push_back(std::make_shared<TNode>(int(i)));
        }

        TTree tree;
        Shuffle(nodes.begin(), nodes.end());

        for (size_t i = 0; i < N; ++i) {
            tree.Insert(nodes[i].get());
        }
        {
            size_t index = 0;
            for (auto node = tree.Begin(); node != tree.End(); ++node, ++index) {
                UNIT_ASSERT_VALUES_EQUAL(tree.GetIndex(&*node), index);
                UNIT_ASSERT_VALUES_EQUAL(tree.ByIndex(index)->N, node->N);
                UNIT_ASSERT_VALUES_EQUAL(node->N, index);
            }
        }

        for (size_t i = 1; i < N; i += 2) {
            auto* node = tree.Find(i);
            UNIT_ASSERT_VALUES_EQUAL(node->N, i);
            tree.Erase(node);
        }
        {
            size_t index = 0;
            for (auto node = tree.Begin(); node != tree.End(); ++node, ++index) {
                UNIT_ASSERT_VALUES_EQUAL(tree.GetIndex(&*node), index);
                UNIT_ASSERT_VALUES_EQUAL(tree.ByIndex(index)->N, node->N);
                UNIT_ASSERT_VALUES_EQUAL(node->N, 2 * index);
            }
        }
    }

    inline void TestGettingIndexWithEqualValues() {
        std::vector<std::shared_ptr<TNode>> nodes;
        size_t N = 1000;

        for (size_t i = 0; i < N; ++i) {
            nodes.push_back(std::make_shared<TNode>(0));
        }

        TTree tree;

        for (size_t i = 0; i < N; ++i) {
            tree.Insert(nodes[i].get());
        }

        for (size_t i = 0; i < N; ++i) {
            UNIT_ASSERT_EQUAL(tree.LessCount(nodes[i]->N), 0);
            UNIT_ASSERT_EQUAL(tree.NotGreaterCount(nodes[i]->N), N);
            UNIT_ASSERT_EQUAL(tree.GreaterCount(nodes[i]->N), 0);
            UNIT_ASSERT_EQUAL(tree.NotLessCount(nodes[i]->N), N);

            UNIT_ASSERT_EQUAL(tree.LessCount(*nodes[i].get()), 0);
            UNIT_ASSERT_EQUAL(tree.NotGreaterCount(*nodes[i].get()), N);
            UNIT_ASSERT_EQUAL(tree.GreaterCount(*nodes[i].get()), 0);
            UNIT_ASSERT_EQUAL(tree.NotLessCount(*nodes[i].get()), N);
        }
    }

    inline void TestFind() {
        TTree tree;

        {
            TNode n1(1);
            TNode n2(2);
            TNode n3(3);

            tree.Insert(n1);
            tree.Insert(n2);
            tree.Insert(n3);

            UNIT_ASSERT_EQUAL(tree.Find(1)->N, 1);
            UNIT_ASSERT_EQUAL(tree.Find(2)->N, 2);
            UNIT_ASSERT_EQUAL(tree.Find(3)->N, 3);

            UNIT_ASSERT(!tree.Find(0));
            UNIT_ASSERT(!tree.Find(4));
            UNIT_ASSERT(!tree.Find(1234567));
        }

        UNIT_ASSERT(tree.Empty());
    }

    inline void TestEmpty() {
        TTree tree;

        UNIT_ASSERT(tree.Empty());
        UNIT_ASSERT_EQUAL(tree.Begin(), tree.End());
    }

    inline void TestInsert() {
        TTree tree;

        {
            TNode n1(1);
            TNode n2(2);
            TNode n3(3);

            tree.Insert(n1);
            tree.Insert(n2);
            tree.Insert(n3);

            TTree::TConstIterator it = tree.Begin();

            UNIT_ASSERT_EQUAL((it++)->N, 1);
            UNIT_ASSERT_EQUAL((it++)->N, 2);
            UNIT_ASSERT_EQUAL((it++)->N, 3);
            UNIT_ASSERT_EQUAL(it, tree.End());
        }

        UNIT_ASSERT(tree.Empty());
    }

    inline void TestErase() {
        TTree tree;

        {
            TNode n1(1);
            TNode n2(2);
            TNode n3(3);

            tree.Insert(n1);
            tree.Insert(n2);
            tree.Insert(n3);

            TTree::TIterator it = tree.Begin();

            tree.Erase(it++);

            UNIT_ASSERT_EQUAL(it, tree.Begin());
            UNIT_ASSERT_EQUAL(it->N, 2);

            tree.Erase(it++);

            UNIT_ASSERT_EQUAL(it, tree.Begin());
            UNIT_ASSERT_EQUAL(it->N, 3);

            tree.Erase(it++);

            UNIT_ASSERT_EQUAL(it, tree.Begin());
            UNIT_ASSERT_EQUAL(it, tree.End());
        }

        UNIT_ASSERT(tree.Empty());
    }

    inline void TestLessCountOnEmptyTree() {
        TTree tree;
        UNIT_ASSERT_VALUES_EQUAL(0, tree.LessCount(TNode(1)));
    }
};

UNIT_TEST_SUITE_REGISTRATION(TRedBlackTreeTest);