statechart APP {
  
  events {
    eCLICK_LOGIN;
    eCLICK_SIGNUP;
    eLOGIN_SUCCESS;
    eSIGNUP_SUCCESS;
    eLOGOUT;
    eBEGIN_ONBOARD;
  }

  // Top-level local variables
  t : int : local;
  onboard : boolean : local;

  entry : {}
  exit  : {}

  functions {
    signup<||>(u: string : local, p: string : local) : int;
    login<||>(u: string : local, p: string : local) : int;
    logout<||>(t: int : local) : int;
  }

  state NOTLOGGEDIN {
  
    entry : {}
    exit  : {}

    state INIT {
      entry : {}
      exit  : {}
    }

    state LOGIN {
      // Local variables for LOGIN now as strings.
      u : string : local;
      p : string : local;
      r : int : local;

      entry : {
        u := "";
        p := "";
        r := 0;
      }
      exit : {}

      state INIT {
        entry : {}
        exit  : {}
      }
      
      state JOIN {
        entry : {}
        exit  : {}
      }
      
      state FAILURE {
        entry : {}
        exit  : {}
      }
      
      // From LOGIN.INIT, clicking login attempts authentication.
      transition t1 {
        source: APP.NOTLOGGEDIN.LOGIN.INIT;
        destination: APP.NOTLOGGEDIN.LOGIN.JOIN;
        trigger: eCLICK_LOGIN;
        guard: true;
        action: { r := login<||>(u, p); }
      }
      
      // From LOGIN.JOIN, if login fails, go to FAILURE.
      transition t3 {
        source: APP.NOTLOGGEDIN.LOGIN.JOIN;
        destination: APP.NOTLOGGEDIN.LOGIN.FAILURE;
        trigger: eLOGIN_SUCCESS;
        guard: (r = 0);
        action: { }
      }
      
      // From FAILURE, clicking login retries the process.
      transition t4 {
        source: APP.NOTLOGGEDIN.LOGIN.FAILURE;
        destination: APP.NOTLOGGEDIN.LOGIN.INIT;
        trigger: eCLICK_LOGIN;
        guard: true;
        action: { }
      }
    }

    state SIGNUP {
      // Local variables for SIGNUP now as strings.
      u : string : local;
      p : string : local;
      r : int : local;

      entry : {
        u := "";
        p := "";
        r := 0;
      }
      exit : {}

      state INIT {
        entry : {}
        exit  : {}
      }
      
      state JOIN {
        entry : {}
        exit  : {}
      }
      
      state SUCCESS {
        entry : {}
        exit  : {}
      }
      
      state FAILURE {
        entry : {}
        exit  : {}
      }
      
      // From SIGNUP.INIT, clicking signup attempts registration.
      transition t5 {
        source: APP.NOTLOGGEDIN.SIGNUP.INIT;
        destination: APP.NOTLOGGEDIN.SIGNUP.JOIN;
        trigger: eCLICK_SIGNUP;
        guard: true;
        action: { r := signup<||>(u, p); }
      }
      
      // If signup succeeds, move to SUCCESS.
      transition t6 {
        source: APP.NOTLOGGEDIN.SIGNUP.JOIN;
        destination: APP.NOTLOGGEDIN.SIGNUP.SUCCESS;
        trigger: eSIGNUP_SUCCESS;
        guard: (r = 1);
        action: { }
      }
      
      // If signup fails, move to FAILURE.
      transition t7 {
        source: APP.NOTLOGGEDIN.SIGNUP.JOIN;
        destination: APP.NOTLOGGEDIN.SIGNUP.FAILURE;
        trigger: eSIGNUP_SUCCESS;
        guard: (r = 0);
        action: { }
      }
      
      // From FAILURE, clicking signup retries the process.
      transition t8 {
        source: APP.NOTLOGGEDIN.SIGNUP.FAILURE;
        destination: APP.NOTLOGGEDIN.SIGNUP.INIT;
        trigger: eCLICK_SIGNUP;
        guard: true;
        action: { }
      }
    }
    
    // On successful signup, return to INIT (login page) so user can log in.
    transition t9 {
      source: APP.NOTLOGGEDIN.SIGNUP.SUCCESS;
      destination: APP.NOTLOGGEDIN.INIT;
      trigger: eCLICK_LOGIN;
      guard: true;
      action: { }
    }
    
    // From INIT, the user may choose to go to LOGIN or SIGNUP.
    transition t10 {
      source: APP.NOTLOGGEDIN.INIT;
      destination: APP.NOTLOGGEDIN.LOGIN;
      trigger: eCLICK_LOGIN;
      guard: true;
      action: { }
    }
    
    transition t11 {
      source: APP.NOTLOGGEDIN.INIT;
      destination: APP.NOTLOGGEDIN.SIGNUP;
      trigger: eCLICK_SIGNUP;
      guard: true;
      action: { }
    }
    
    // Transition from LOGIN to SIGNUP (if user clicks "create an account" link).
    transition t14 {
      source: APP.NOTLOGGEDIN.LOGIN;
      destination: APP.NOTLOGGEDIN.SIGNUP;
      trigger: eCLICK_SIGNUP;
      guard: true;
      action: { }
    }
  }

  state LOGGEDIN {
  
    entry : { onboard := false; }
    exit  : {}

    state MAIN {
      entry : {}
      exit  : {}
    }
    
    state ONBOARD {
      entry : {}
      exit  : {}
    }
    
    state MAPPAGE {
      entry : {}
      exit  : {}
    }
    
    // In LOGGEDIN, if the user has not been onboarded, trigger onboarding.
    transition t12 {
      source: APP.LOGGEDIN.MAIN;
      destination: APP.LOGGEDIN.ONBOARD;
      trigger: eBEGIN_ONBOARD;
      guard: (onboard = false);
      action: { onboard := true; }
    }

      // If already onboarded, go directly to MAPPAGE.
    transition t15 {
      source: APP.LOGGEDIN.MAIN;
      destination: APP.LOGGEDIN.MAPPAGE;
      trigger: eBEGIN_ONBOARD;
      guard: (onboard = true);
      action: { }
    }
  }

  
  // --- Top-Level Transitions ---
  
  // Transition from LOGIN.JOIN to LOGGEDIN on successful login.
  transition t2 {
    source: APP.NOTLOGGEDIN.LOGIN.JOIN;
    destination: APP.LOGGEDIN;
    trigger: eLOGIN_SUCCESS;
    guard: (r = 1);
    action: { }
  }
  
  // Logout transition from LOGGEDIN back to NOTLOGGEDIN.
  transition t13 {
    source: APP.LOGGEDIN;
    destination: APP.NOTLOGGEDIN;
    trigger: eLOGOUT;
    guard: true;
    action: { logout<||>(t); }
  }
}