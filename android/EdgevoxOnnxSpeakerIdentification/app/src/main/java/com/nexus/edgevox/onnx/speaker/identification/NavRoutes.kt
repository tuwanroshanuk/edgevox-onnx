package com.nexus.edgevox.onnx.speaker.identification

sealed class NavRoutes(val route: String) {
    object Home : NavRoutes("home")
    object Register : NavRoutes("register")
    object View : NavRoutes("view")
    object Help : NavRoutes("help")
}