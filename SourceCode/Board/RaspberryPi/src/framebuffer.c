//
// Created by XingfengYang on 2020/7/7.
//

#include <framebuffer.h>
#include <kheap.h>
#include <log.h>
#include <mailbox.h>
#include <stdlib.h>

uint32_t pitch;
unsigned char *lfb;

uint32_t framebufferWidth;
uint32_t framebufferHeight;

volatile uint32_t __attribute__((aligned(16))) mailbox[36];

uint32_t framebuffer_get_width(void){return framebufferWidth;}

uint32_t framebuffer_get_height(void){return framebufferHeight;}

int framebuffer_init(void) {
  /*
  framebufferWidth = 1024;
  framebufferHeight = 768;
  PropertySetPhysicalDisplayWHMail *setPhysicalDisplayWHMail =
      kheap_alloc_aligned(sizeof(PropertySetPhysicalDisplayWHMail), 16);
  setPhysicalDisplayWHMail->size = sizeof(PropertySetPhysicalDisplayWHMail);
  setPhysicalDisplayWHMail->code = CODE_REQUEST;
  setPhysicalDisplayWHMail->end = PROPERTY_TAG_END;
  setPhysicalDisplayWHMail->property.tag = PROPERTY_TAG_SET_PHYSICAL_DISPLAY_WH;
  setPhysicalDisplayWHMail->property.bufferSize = 8;
  setPhysicalDisplayWHMail->property.code = 0x80000000 + 8;
  setPhysicalDisplayWHMail->property.width = framebufferWidth;
  setPhysicalDisplayWHMail->property.height = framebufferHeight;
  MailMessage setPhysicalDisplayWHMailMsg = {.channel = MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VC,
                                             .data = setPhysicalDisplayWHMail};
  mailbox_call(setPhysicalDisplayWHMailMsg);
  if (setPhysicalDisplayWHMail->code == CODE_RESPONSE_FAILURE ||
      setPhysicalDisplayWHMail->property.code == CODE_RESPONSE_FAILURE) {
    LogError("[Framebuffer]: Unable to set physical WH\n");
  }
  */

  PropertyGetPhysicalDisplayWHMail *getPhysicalDisplayWHMail =
      kheap_alloc_aligned(sizeof(PropertyGetPhysicalDisplayWHMail), 16);
  getPhysicalDisplayWHMail->size = sizeof(PropertyGetPhysicalDisplayWHMail);
  getPhysicalDisplayWHMail->code = CODE_REQUEST;
  getPhysicalDisplayWHMail->end = PROPERTY_TAG_END;
  getPhysicalDisplayWHMail->property.tag = PROPERTY_TAG_GET_PHYSICAL_DISPLAY_WH;
  getPhysicalDisplayWHMail->property.bufferSize = 8;
  getPhysicalDisplayWHMail->property.code = 0x80000000 + 8;
  getPhysicalDisplayWHMail->property.width = 0;
  getPhysicalDisplayWHMail->property.height = 0;
  MailMessage setPhysicalDisplayWHMailMsg = {.channel = MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VC,
                                             .data = getPhysicalDisplayWHMail};
  mailbox_call(setPhysicalDisplayWHMailMsg);
  if (getPhysicalDisplayWHMail->code == CODE_RESPONSE_FAILURE ||
      getPhysicalDisplayWHMail->property.code == CODE_RESPONSE_FAILURE) {
    LogError("[Framebuffer]: Unable to set physical WH\n");
  }
  framebufferWidth = getPhysicalDisplayWHMail->property.width;
  framebufferHeight = getPhysicalDisplayWHMail->property.height;

  PropertySetVirtualBufferWHMail *setVirtualBufferWHMail =
      kheap_alloc_aligned(sizeof(PropertySetVirtualBufferWHMail), 16);
  setVirtualBufferWHMail->size = sizeof(PropertySetVirtualBufferWHMail);
  setVirtualBufferWHMail->code = CODE_REQUEST;
  setVirtualBufferWHMail->end = PROPERTY_TAG_END;
  setVirtualBufferWHMail->property.tag = PROPERTY_TAG_SET_VIRTUAL_BUFFER_WH;
  setVirtualBufferWHMail->property.bufferSize = 8;
  setVirtualBufferWHMail->property.code = 0x80000000 + 8;
  setVirtualBufferWHMail->property.width = framebufferWidth;
  setVirtualBufferWHMail->property.height = framebufferHeight;
  MailMessage setVirtualBufferWHMailMsg = {.channel = MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VC,
                                           .data = setVirtualBufferWHMail};
  mailbox_call(setVirtualBufferWHMailMsg);
  if (setVirtualBufferWHMail->code == CODE_RESPONSE_FAILURE ||
      setVirtualBufferWHMail->property.code == CODE_RESPONSE_FAILURE) {
    LogError("[Framebuffer]: Unable to set virtual WH\n");
  }

  PropertySetDepthMail *setDepthMail = kheap_alloc_aligned(sizeof(PropertySetDepthMail), 16);
  setDepthMail->size = sizeof(PropertySetDepthMail);
  setDepthMail->code = CODE_REQUEST;
  setDepthMail->end = PROPERTY_TAG_END;
  setDepthMail->property.tag = PROPERTY_TAG_SET_DEPTH;
  setDepthMail->property.bufferSize = 4;
  setDepthMail->property.code = 0x80000000 + 4;
  setDepthMail->property.bitsPerPixel = 32;
  MailMessage setDepthMailMsg = {.channel = MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VC, .data = setDepthMail};
  mailbox_call(setDepthMailMsg);
  if (setDepthMail->code == CODE_RESPONSE_FAILURE || setDepthMail->property.code == CODE_RESPONSE_FAILURE) {
    LogError("[Framebuffer]: Unable to set depth\n");
  }

  PropertySetVirtualOffsetMail *setVirtualOffsetMail = kheap_alloc_aligned(sizeof(PropertySetVirtualOffsetMail), 16);
  setVirtualOffsetMail->size = sizeof(PropertySetVirtualOffsetMail);
  setVirtualOffsetMail->code = CODE_REQUEST;
  setVirtualOffsetMail->end = PROPERTY_TAG_END;
  setVirtualOffsetMail->property.tag = PROPERTY_TAG_SET_VIRTUAL_OFFSET;
  setVirtualOffsetMail->property.bufferSize = 8;
  setVirtualOffsetMail->property.code = 0x80000000 + 8;
  setVirtualOffsetMail->property.xOffset = 0;
  setVirtualOffsetMail->property.yOffset = 0;
  MailMessage setVirtualOffsetMailMsg = {.channel = MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VC,
                                         .data = setVirtualOffsetMail};
  mailbox_call(setVirtualOffsetMailMsg);
  if (setVirtualOffsetMail->code == CODE_RESPONSE_FAILURE ||
      setVirtualOffsetMail->property.code == CODE_RESPONSE_FAILURE) {
    LogError("[Framebuffer]: Unable to set virtual offset\n");
  }

  PropertyAllocateBufferMail *allocateBufferMail = kheap_alloc_aligned(sizeof(PropertyAllocateBufferMail), 16);
  allocateBufferMail->size = sizeof(PropertyAllocateBufferMail);
  allocateBufferMail->code = CODE_REQUEST;
  allocateBufferMail->end = PROPERTY_TAG_END;
  allocateBufferMail->property.tag = PROPERTY_TAG_ALLOCATE_BUFFER;
  allocateBufferMail->property.bufferSize = 8;
  allocateBufferMail->property.code = 0x80000000 + 4;
  allocateBufferMail->property.PACKED.alignment = 16;
  allocateBufferMail->property.size = 0;
  MailMessage allocateBufferMailMsg = {.channel = MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VC, .data =
  allocateBufferMail}; mailbox_call(allocateBufferMailMsg); if (allocateBufferMail->code == CODE_RESPONSE_FAILURE ||
  allocateBufferMail->property.code == CODE_RESPONSE_FAILURE) {
    LogError("[Framebuffer]: Unable to allocate video buffer\n");
  }

  PropertyGetPitchMail *getPitchMail = kheap_alloc_aligned(sizeof(PropertyGetPitchMail), 16);
  getPitchMail->size = sizeof(PropertyGetPitchMail);
  getPitchMail->code = CODE_REQUEST;
  getPitchMail->end = PROPERTY_TAG_END;
  getPitchMail->property.tag = PROPERTY_TAG_GET_PITCH;
  getPitchMail->property.bufferSize = 4;
  getPitchMail->property.code = 0;
  getPitchMail->property.bytesPerLine = 0;
  MailMessage getPitchMailMsg = {.channel = MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VC, .data = getPitchMail};
  mailbox_call(getPitchMailMsg);
  if (getPitchMail->code == CODE_RESPONSE_FAILURE || getPitchMail->property.code == CODE_RESPONSE_FAILURE) {
    LogError("[Framebuffer]: Unable to get pitch\n");
  }

  if (setDepthMail->property.bitsPerPixel == 32 && allocateBufferMail->property.PACKED.baseAddress != 0) {
    allocateBufferMail->property.PACKED.baseAddress &= 0x3FFFFFFF;
    pitch = getPitchMail->property.bytesPerLine;
    lfb = (void *)((uint64_t)allocateBufferMail->property.PACKED.baseAddress);
    LogInfo("[Framebuffer]: inited framebuffer %d x %d x %d\n", framebufferWidth, framebufferHeight, setDepthMail->property.bitsPerPixel);
  } else {
    LogError("[Framebuffer]: Unable to init framebuffer\n");
  }

  /*
  mailbox[0] = 35 * 4;
  mailbox[1] = CODE_REQUEST;
  mailbox[2] = 0x48003; // set phy wh
  mailbox[3] = 8;
  mailbox[4] = 8;
  mailbox[5] = 1024;    // FrameBufferInfo.width
  mailbox[6] = 768;     // FrameBufferInfo.height
  mailbox[7] = 0x48004; // set virt wh
  mailbox[8] = 8;
  mailbox[9] = 8;
  mailbox[10] = 1024; // FrameBufferInfo.virtual_width
  mailbox[11] = 768;  // FrameBufferInfo.virtual_height

  mailbox[12] = 0x48009; // set virt offset
  mailbox[13] = 8;
  mailbox[14] = 8;
  mailbox[15] = 0; // FrameBufferInfo.x_offset
  mailbox[16] = 0; // FrameBufferInfo.y.offset

  mailbox[17] = 0x48005; // set depth
  mailbox[18] = 4;
  mailbox[19] = 4;
  mailbox[20] = 32;      // FrameBufferInfo.depth
  mailbox[21] = 0x48006; // set pixel order
  mailbox[22] = 4;
  mailbox[23] = 4;
  mailbox[24] = 1;       // RGB, not BGR preferably
  mailbox[25] = 0x40001; // get framebuffer, gets alignment on request
  mailbox[26] = 8;
  mailbox[27] = 8;
  mailbox[28] = 4096;    // FrameBufferInfo.pointer
  mailbox[29] = 0;       // FrameBufferInfo.size
  mailbox[30] = 0x40008; // get pitch
  mailbox[31] = 4;
  mailbox[32] = 4;
  mailbox[33] = 0; // FrameBufferInfo.pitch

  mailbox[34] = PROPERTY_TAG_END;

  MailMessage msg = {.channel = MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VC, .data = mailbox};
  mailbox_call(msg);
  if (mailbox[20] == 32 && mailbox[28] != 0) {
    mailbox[28] &= 0x3FFFFFFF;
    framebufferWidth = mailbox[5];
    framebufferHeight = mailbox[6];
    pitch = mailbox[33];
    lfb = (void *)((uint64_t)mailbox[28]);
  } else {
    LogError("[Framebuffer]: Unable to set screen resolution to 1024x768x32\n");
  }
  */
}

void framebuffer_draw_pixel(uint32_t x, uint32_t y, char r, char g, char b) {
  unsigned char *ptr = lfb;
  uint32_t pixel;

  ptr += pitch * y;
  ptr += (x << 2);

  pixel = (b << 16) | (g << 8) | r;

  *((uint32_t *)ptr) = *((uint32_t *)&pixel);
}

void framebuffer_clear(uint32_t color) {
  unsigned char *ptr = lfb;
  // Clearing the whole screen
  for (uint32_t y = 0; y < framebufferHeight; y++) {
    for (uint32_t x = 0; x < framebufferWidth; x++) {
      *((uint32_t *)ptr) = color;
      ptr += 4;
    }
    ptr += pitch - framebufferWidth * 4;
  }
}
