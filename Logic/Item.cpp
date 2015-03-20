<<<<<<< HEAD
#pragma once
#include <sstream>
#include <iostream>
#include "DateTime.cpp"
=======
#include "Item.h"
>>>>>>> master

Item::Item() {
	event = "";
	eventDate[0] = 0;
	eventDate[1] = 0;
	eventDate[2] = 0;
	eventStartTime[0] = 0;
	eventStartTime[1] = 0;
	eventEndTime[0] = 0;
	eventEndTime[1] = 0;
	colour = 0;
	bold = false;
}

Item::~Item() {
}

int Item::getHour(int hour) {
	if(hour == 24) {
		return 0;
	} else if(hour > 12 && hour < 24) {
		return hour - 12;
	} else {
		return hour;
	}
}

string Item::getMinute(int minute) {
	if( minute == 0) {
		return "00";
	} else {
		return to_string(minute);
	}
}

<<<<<<< HEAD
	string getMinute(int minute) {
		try{
		  if(minute == 0) {
		     return "00";
		} else {
			return to_string(minute);
		}
		  if(minute < 0) throw minute;
		  if(minute == NULL)throw "allocation failure";
		}catch(int i){
		cout << "Exception: ";
		cout << "The negative minute "<< minute << " is invalid"<<endl;
		}catch(char *str){
		cout << "Exception: ";
		cout << str <<endl;
		}
}

	
	string dateToString() {
		ostringstream oss;
		if((eventDate[0] == 0) && (eventDate[1] == 0) && (eventDate[2] == 0)) {
			return "";
        
		} else if((eventDate[0] == 0)&&(eventDate[1] != 0) && (eventDate[2] != 0)) {
			oss << "[" << itemDate.getWeekDay(eventDate[0], eventDate[1], eventDate[2]);
			oss << ", " << eventDate[0] << "/" << eventDate[1] << "/" << eventDate[2];
			return oss.str();
		
		} else if((eventDate[0] == 0)&&(eventDate[1] != 0) && (eventDate[2] != 0)) {
			oss << "[  /" << eventDate[1] << "/" << eventDate[2];
			return oss.str();
		
		}else if((eventDate[0] != 0) && (eventDate[1] == 0) && (eventDate[2] != 0)){
			return "";
	
		}else if((eventDate[0] != 0) && (eventDate[1] != 0) && (eventDate[2] == 0)){    
			oss << "[" << itemDate.getWeekDay(eventDate[0], eventDate[1], eventDate[2]);
			oss << ", " << eventDate[0] << "/" << eventDate[1];
		
		}else if((eventDate[0] == 0) && (eventDate[1] != 0) && (eventDate[2] == 0)){
			oss << "[ /" << eventDate[1] ;
		
		}else if((eventDate[0] != 0) && (eventDate[1] == 0) && (eventDate[2] == 0)){
	     return "";		
		}
=======
string Item::getAMPM(int hour) {
	if(hour >= 12 && hour < 24) {
		return "pm";
	} else {
		return "am";
	}
}

string Item::dateToString() {
	if((eventDate[0] == 0) && (eventDate[1] == 0) && (eventDate[1] == 0)) {
		return "";
	} else {
		ostringstream oss;
		oss << itemDate.getWeekDay(eventDate[0], eventDate[1], eventDate[2]);
		oss << ", " << eventDate[0] << " ";
		oss << itemDate.getMonth(eventDate[1]) << " " << eventDate[2];
		return oss.str();
>>>>>>> master
	}
}

<<<<<<< HEAD
	string timeToString() {
		ostringstream oss;
		int startHour;
		int endHour;

		if((eventStartTime[0] == -1) && (eventEndTime[0] == -1)) {
			return "";
		
		}else if((eventStartTime[0] != -1) && (eventEndTime[0] != -1)){
			if(eventStartTime[0] == 0){
			startHour = 12;
	     	}else if(eventStartTime[0] > 12){
		    startHour = eventStartTime[0] - 12;
		    }
		    
		    if(eventEndTime[0] == 0){
			endHour = 12;
			}else if(eventEndTime[0] > 12){
			endHour = eventEndTime[0] - 12;
			}
	    
		oss << ", " << eventStartTime[0] << ":" << getMinute(eventStartTime[1]);
		oss << " - " << eventEndTime[0] << ":" << getMinute(eventEndTime[1])<<"]";

		}else if((eventStartTime[0] != -1) && (eventEndTime[0] == -1)){
			if(eventStartTime[0] == 0){
			startHour = 12;
	     	}else if(eventStartTime[0] > 12){
		    startHour = eventStartTime[0] - 12;
			} 
		oss << ", " << eventStartTime[0] << ":" << getMinute(eventStartTime[1])<<"]";

		}else if((eventStartTime[0] == -1) && (eventEndTime[0] != -1)){
			return "";
		}

	}
=======
string Item::timeToString() {
	ostringstream oss;
	if(eventStartTime[0] == 0) {
		return "";
	} else {
		string startTimeOfDay = getAMPM(eventStartTime[0]);
		oss << "[" << getHour(eventStartTime[0]) << ":" ;
		oss << getMinute(eventStartTime[1]) << " " << startTimeOfDay;

		if(eventEndTime[0] != 0) {
			string endTimeOfDay = getAMPM(eventEndTime[0]);
			oss << " - " << getHour(eventEndTime[0]) << ":";
			oss << getMinute(eventEndTime[1]) << " " << endTimeOfDay;
		}
		oss << "]";
>>>>>>> master

		return oss.str();
	}
}

string Item::toString() {
	ostringstream oss;
	oss << event << dateToString() << timeToString() << endl;
	return oss.str();
}
