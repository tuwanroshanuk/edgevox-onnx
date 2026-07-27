//
//  ContentView.swift
//  EdgevoxOnnx
//
//  Created by fangjun on 2023/4/5.
//

import SwiftUI

struct ContentView: View {
    @StateObject var edgevoxOnnxVM = EdgevoxOnnxViewModel()

    var body: some View {
        VStack {
            Text("ASR with Next-gen Kaldi")
                .font(.title)
            if edgevoxOnnxVM.status == .stop {
                Text("See https://github.com/k2-fsa/edgevox-onnx")
                Text("Press the Start button to run!")
            }
            ScrollView(.vertical, showsIndicators: true) {
                HStack {
                    Text(edgevoxOnnxVM.subtitles)
                    Spacer()
                }
            }
            Spacer()
            Button {
                toggleRecorder()
            } label: {
                Text(edgevoxOnnxVM.status == .stop ? "Start" : "Stop")
            }
        }
        .padding()
    }

    private func toggleRecorder() {
        edgevoxOnnxVM.toggleRecorder()
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
