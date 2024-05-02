#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	for (int radius = 50; radius <= 250; radius += 10) {

		this->radius_list.push_back(radius);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->frame.clear();

	ofSeedRandom(39);
	glm::vec3 noise_param = glm::vec3(ofRandom(360), ofRandom(360), ofRandom(360));

	for (int i = 0; i < this->radius_list[i]; i++) {

		auto rotation = glm::vec3(
			ofMap(ofNoise(noise_param.x, this->radius_list[i] * 0.0009 + ofGetFrameNum() * 0.003), 0, 1, -360, 360),
			ofMap(ofNoise(noise_param.y, this->radius_list[i] * 0.0009 + ofGetFrameNum() * 0.003), 0, 1, -360, 360),
			ofMap(ofNoise(noise_param.z, this->radius_list[i] * 0.0009 + ofGetFrameNum() * 0.003), 0, 1, -360, 360)
		);

		this->setRingToMesh(this->face, this->frame, glm::vec3(), rotation, this->radius_list[i], this->radius_list[i] * 0.1, ofColor(255, 0, 255, 64), ofColor(255, 0, 0));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.44);

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();

	/*
	int start = 300;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, glm::vec3 rotation, float radius, float height, ofColor face_color, ofColor frame_color) {

	int index = face_target.getNumVertices();

	for (int deg = 0; deg < 360; deg += 5) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 5) * DEG_TO_RAD), radius * sin((deg + 5) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 5) * DEG_TO_RAD), radius * sin((deg + 5) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			auto rotation_x = glm::rotate(glm::mat4(), rotation.x * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), rotation.y * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), rotation.z * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

			vertex = glm::vec4(vertex, 0) * rotation_y * rotation_x + glm::vec4(location, 0);
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}