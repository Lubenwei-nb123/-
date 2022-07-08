package main

import (
	"crypto/sha256"
	"fmt"
)

//定义Merkle树的节点结构体
type MerkleNode struct {
	left  *MerkleNode
	right *MerkleNode
	data  []byte
}

//定义Merkle树结构
type MerkleTree struct {
	root *MerkleNode //只记录一个根节点即可追溯全部节点.
}

//创建节点
func NewMerkleNode(left, right *MerkleNode, data []byte) *MerkleNode {
	NewNode := MerkleNode{}
	//如果为左右节点空，那么就说明他是原始数据节点（叶子节点）。
	if left == nil && right == nil {
		hash := sha256.Sum256(data)
		NewNode.data = hash[:]
	} else {
		prevHashes := append(left.data, right.data...)
		hash := sha256.Sum256(prevHashes)
		NewNode.data = hash[:]
	}
	NewNode.left = left
	NewNode.right = right
	return &NewNode
}

func NewMerkleTree(data [][]byte) *MerkleTree {
	var nodes []MerkleNode
	//这里要保证节点数为偶数
	if len(data)%2 != 0 {
		data = append(data, data[len(data)-1])
	}
	for _, datum := range data {
		node := NewMerkleNode(nil, nil, datum)
		nodes = append(nodes, *node)
	}
	//循环嵌套完成节点树形构造
	for i := 0; i < len(data)/2; i++ {
		var newLevel []MerkleNode
		for j := 0; j < len(nodes); j += 2 {
			node := NewMerkleNode(&nodes[j], &nodes[j+1], nil)
			newLevel = append(newLevel, *node)
		}
		nodes = newLevel
	}
	//构造Merkle树
	NewTree := MerkleTree{&nodes[0]}
	return &NewTree
}

func showMerkleTree(root *MerkleNode) {
	if root == nil {
		return
	} else {
		PrintNode(root)
	}
	showMerkleTree(root.left)
	showMerkleTree(root.right)
}

func PrintNode(node *MerkleNode) {
	fmt.Printf("%p\n", node)
	if node != nil {
		fmt.Printf("left[%p],right[%p],data(%x)\n", node.left, node.right, node.data)
	}
}

func main() {
	data := [][]byte{
		[]byte("SDU"),
		[]byte("is"),
		[]byte("No.1"),
		[]byte("university"),
	}
	tree := NewMerkleTree(data)
	showMerkleTree(tree.root)
}
