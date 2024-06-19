#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// # include "../includes/Main.hpp"
// # include "../includes/Server.hpp"
# include "Server.hpp"

class Channel
{
	private:
		std::string                 _channelName;
		
		std::vector<int>            _operatorFdList;
		std::vector<int>            _clientFdList;
		std::vector<int>            _inviteFdList;
		std::string                 _topic;
		std::string                 _key;

		bool                        _inviteStatus;
		bool                        _topicStatus;
		bool                        _keyStatus;
		bool                        _limitStatus;

		unsigned int                _limit;

	public:
		Channel();
		Channel(const std::string&, int);
		Channel(const Channel &);
		Channel &operator=(const Channel &);
		~Channel();

		// getters
		std::string                 getChannelName();
		std::vector<int>            getFdListClient();
		std::vector<int>            getFdListOperator();
		std::string                 getMode();
		std::string                 getTopic();
		unsigned int                getLimit();
		std::string                 getKey();
		std::vector<int>::iterator  findIterClient(int);
		// Bot*                        getBot();

		// setters
		void                        setChannelName(std::string&);
		// void						setMode(unsigned char, char);	// 구버전
		void                        setMode(unsigned char, char, int);	//fd 있는 버전
		void                        setLimit(unsigned int);
		void                        setTopic(std::string);
		void                        setKey(std::string);
		void                        appendFdListClient(int);
		void                        removeFdListClient(int);
		void                        addOperatorFd(int);
		void                        removeOperatorFd(int);
		void                        appendFdListInvite(int);
		void						setOperator(int, char);

		// diff
		bool						diffMode(unsigned char);
		bool                        diffInvite(int);
		bool                        diffKey(std::string);
		bool                        diffClientInChannel(int);
		bool                        diffOperator(int);	

		std::string					makeMsgForm(int);
		void						messageAllChannel(int, std::string, std::string, std::string);
};

#endif
