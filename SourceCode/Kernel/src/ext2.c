//
// Created by XingfengYang on 2020/8/7.
//

#include <ext2.h>
#include <log.h>
#include <stdint.h>

extern char _binary_initrd_img_start[];
extern char _binary_initrd_img_end[];
extern char _binary_initrd_img_size[];

#define EXT2_SIGNATURE 0xef53
#define EXT2_BLOCK_GROUP_DESCRIPTOR_SIZE 32
#define EXT2_INDEX_NODE_STRUCTURE_SIZE 128


uint32_t EXT2_ADDRESS = _binary_initrd_img_start;

KernelStatus ext2_init() {
  Ext2SuperBlock *ext2SuperBlock = (Ext2SuperBlock *)(EXT2_ADDRESS + 1024);

  if(ext2SuperBlock->signature != EXT2_SIGNATURE){
    LogError("[Ext2]: not a ext2 file system.\n");
    return ERROR;
  }

  LogInfo("[Ext2]: %d inodes in file system.\n", ext2SuperBlock->indexNodeNums);
  LogInfo("[Ext2]: %d blocks in file system.\n", ext2SuperBlock->blockNums);
  LogInfo("[Ext2]: %d blocks reserved for superuser.\n", ext2SuperBlock->blockReservedForSuperuserNums);
  LogInfo("[Ext2]: %d unallocated blocks.\n", ext2SuperBlock->unallocatedBlockNums);
  LogInfo("[Ext2]: %d unallocated inodes.\n", ext2SuperBlock->unallocatedIndexNodeNums);
  LogInfo("[Ext2]: %d block containing the superblock.\n", ext2SuperBlock->blockContainingSuperblockNums);
  LogInfo("[Ext2]: BlockSize: %d .\n", 1 << (ext2SuperBlock->log2BlockSizeSub10 + 10));
  LogInfo("[Ext2]: FragmentSize: %d .\n", 1 << (ext2SuperBlock->log2FragmentSizeSub10 + 10));
  LogInfo("[Ext2]: %d blocks in each block group.\n", ext2SuperBlock->eachBlockGroupBlockNums);
  LogInfo("[Ext2]: %d fragments in each block group.\n", ext2SuperBlock->eachBlockGroupFragmentNums);
  LogInfo("[Ext2]: %d inodes in each block group.\n", ext2SuperBlock->eachBlockGroupIndexNodeNums);
  LogInfo("[Ext2]: LastMountTime: %d .\n", ext2SuperBlock->lastMountTime);
  LogInfo("[Ext2]: LastWrittenTime: %d .\n", ext2SuperBlock->lastWrittenTime);
  LogInfo("[Ext2]: %d times the volume has been mounted since its last consistency check (fsck)\n",
          ext2SuperBlock->numberOfTimes);
  LogInfo("[Ext2]: %d mounts allowed before a consistency check (fsck) must be done\n", ext2SuperBlock->numberOfMounts);
  LogInfo("[Ext2]: Ext2Signature: %d .\n", ext2SuperBlock->signature);
  LogInfo("[Ext2]: File system state %d .\n", ext2SuperBlock->state);
  LogInfo("[Ext2]: Minor portion of version %d .\n", ext2SuperBlock->minorPortionOfVersion);
  LogInfo("[Ext2]: POSIX time of last consistency check: %d .\n", ext2SuperBlock->lastConsistencyCheckTime);
  LogInfo("[Ext2]: Interval between forced consistency checks: %d .\n",
          ext2SuperBlock->intervalBetweenForcedConsistencyChecks);
  LogInfo("[Ext2]: Operating system ID: %d .\n", ext2SuperBlock->operatingSystemID);
  LogInfo("[Ext2]: Major portion of version: %d .\n", ext2SuperBlock->majorPortionOfVersion);
  LogInfo("[Ext2]: User ID: %d \n", ext2SuperBlock->userId);
  LogInfo("[Ext2]: Group ID %d \n", ext2SuperBlock->groupId);
  LogInfo("[Ext2]: First non-reserved inode in file system: %d .\n", ext2SuperBlock->firstIndexNode);
  LogInfo("[Ext2]: Size of each inode structure in bytes: %d .\n", ext2SuperBlock->indexNodeStructureSize);
  LogInfo("[Ext2]: Superblock is part of Block group: %d .\n", ext2SuperBlock->blockGrousp);
  LogInfo("[Ext2]: Voluma Name %s .\n", (char *)ext2SuperBlock->volumaName);
  LogInfo("[Ext2]: Path volume was last mounted to %s .\n", (char *)ext2SuperBlock->lastMountPath);

  // Block size
  uint32_t blockSize = 1 << (ext2SuperBlock->log2BlockSizeSub10 + 10);

  uint32_t blockNumsInEachBlockGroup = (blockSize * 8);

  // Block Group Descriptor numbers , other words, this is super block numbers or block group number
  uint32_t blockGroupNums =
  ext2SuperBlock->blockNums / blockNumsInEachBlockGroup +
     (ext2SuperBlock->blockNums % blockNumsInEachBlockGroup) > 0 ? 1 : 0;

  uint32_t blockGroupDescriptorNumsInEachBlock = blockSize / EXT2_BLOCK_GROUP_DESCRIPTOR_SIZE;
  // block nums for all block group descriptor
  uint32_t blockForBlockGroupDescriptor = blockGroupNums / blockGroupDescriptorNumsInEachBlock;
  uint32_t blockForBlockGroupDescriptorMod =  (blockGroupNums % blockGroupDescriptorNumsInEachBlock) > 0 ? 1 : 0;
  blockForBlockGroupDescriptor += blockForBlockGroupDescriptorMod;
  LogError("[Ext2]: block group descriptor blocks: %d .\n",blockForBlockGroupDescriptor);

  uint32_t indexNodeStructureNumsInEachBlock = blockSize / EXT2_INDEX_NODE_STRUCTURE_SIZE;
  uint32_t blockForIndexNodeTable = ext2SuperBlock->indexNodeNums / indexNodeStructureNumsInEachBlock;
  uint32_t blockForIndexNodeTableMod =   ((ext2SuperBlock->indexNodeNums % indexNodeStructureNumsInEachBlock) > 0) ? 1 : 0;
  blockForIndexNodeTable += blockForIndexNodeTableMod;
  LogError("[Ext2]: index node table  blocks: %d .\n",blockForIndexNodeTable);

  Ext2BlockGroup blockGroup;
  blockGroup.superBlock = (Ext2SuperBlock*)(EXT2_ADDRESS + 1024);
  blockGroup.blockGroupDescriptor = (Ext2BlockGroupDescriptor*) ((uint32_t)blockGroup.superBlock + blockSize);
  blockGroup.dataBlockBitmap = (Ext2DataBlockBitmap*)((uint32_t)blockGroup.blockGroupDescriptor + blockForBlockGroupDescriptor * blockSize);
  blockGroup.indexNodeBitmap = (Ext2IndexNodeBlockBitmap*)((uint32_t)blockGroup.dataBlockBitmap + blockSize);
  blockGroup.indexNodeDataStructure = (Ext2IndexNodeDataStructure*)((uint32_t)blockGroup.indexNodeBitmap + blockSize);
  blockGroup.dataBlock = (Ext2DataBlock*)((uint32_t)blockGroup.indexNodeDataStructure + blockForIndexNodeTable * blockSize);

  
}
