#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <vector>

struct ChatMessage {
    int senderID;
    int receiverID;
    std::string text;
    std::string timestamp;

    ChatMessage(int s, int r, const std::string& t, const std::string& ts)
        : senderID(s), receiverID(r), text(t), timestamp(ts) {}
};

struct ChatNode {
    ChatMessage message;
    ChatNode* next;
    ChatNode(const ChatMessage& msg) : message(msg), next(NULL) {}
};

#endif
