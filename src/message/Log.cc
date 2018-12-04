#include "Log.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t Log::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_LOG));
  int32_t wRstContent;
  assert(content);
  wRstContent = caps->write(content->c_str());
  if (wRstContent != CAPS_SUCCESS) return wRstContent;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t Log::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_LOG));
  int32_t wRstContent;
  assert(content);
  wRstContent = caps->write(content->c_str());
  if (wRstContent != CAPS_SUCCESS) return wRstContent;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t Log::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  if (!content) content = std::make_shared<std::string>();
  int32_t rRstContent = caps->read_string(*content);
  if (rRstContent != CAPS_SUCCESS) return rRstContent;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t Log::deserialize(std::shared_ptr<Caps> &caps) {
  if (!content) content = std::make_shared<std::string>();
  int32_t rRstContent = caps->read_string(*content);
  if (rRstContent != CAPS_SUCCESS) return rRstContent;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t Log::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstContent;
  assert(content);
  wRstContent = caps->write(content->c_str());
  if (wRstContent != CAPS_SUCCESS) return wRstContent;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t Log::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  if (!content) content = std::make_shared<std::string>();
  int32_t rRstContent = caps->read_string(*content);
  if (rRstContent != CAPS_SUCCESS) return rRstContent;
  return CAPS_SUCCESS;
}

